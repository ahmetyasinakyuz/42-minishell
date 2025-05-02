/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:09:59 by aycami            #+#    #+#             */
/*   Updated: 2025/05/02 18:50:58 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(char **env)
{
	int i = 0;

	if (!env)
		return;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	exit_builtin(t_simple_cmds *cmd_list, char **envp, t_lexer *token_list, pid_t *pids, t_vars **vars)
{
	write(STDOUT_FILENO, "exit\n", 5);
	free_command_list(cmd_list);
	free_lexer_list(token_list);
	free(pids);
	clear_vars(vars);
	free_env(envp);
	rl_clear_history();
    exit(0);
}

void handle_pipe(t_simple_cmds *cmd, t_simple_cmds *next)
{
	int pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	cmd->output_fd = pipe_fd[1];
	cmd->output_type = IO_PIPE_OUT;
	next->input_fd = pipe_fd[0];
	next->input_type = IO_PIPE_IN;
}

void execute(t_simple_cmds *cmd_list, char ***envp, t_lexer *token_list, t_vars **vars)
{
	t_simple_cmds *current_cmd;
	t_simple_cmds *last_cmd;
	pid_t *pids;
	int cmd_count = 0;
	int i;
	int status = 0;  // Initialize status

	// Initialize last_cmd to prevent uninitialized use
	last_cmd = cmd_list;
	current_cmd = cmd_list;
	while (current_cmd)
	{
		cmd_count++;
		// Keep track of the last command for return value
		last_cmd = current_cmd;
		current_cmd = current_cmd->next;
	}
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		perror("malloc");
		return;
	}
	// Initialize all pids to -1 (invalid pid)
	for (i = 0; i < cmd_count; i++)
		pids[i] = -1;

	current_cmd = cmd_list;
	i = 0;
	while (current_cmd)
	{
		if (ft_strncmp("export", *current_cmd->str, 7) == 0)
			export_builtin(current_cmd, envp);
		else if(ft_strncmp("unset", *current_cmd->str, 6) == 0)
			unset_builtin(current_cmd, envp);
		else if(ft_strncmp("cd", *current_cmd->str, 3) == 0)
			cd_builtin(current_cmd);
		else if(ft_strncmp("exit", *current_cmd->str, 5) == 0)
			exit_builtin(current_cmd, *envp, token_list, pids, vars);
		else
		{
			if (current_cmd->next)
				handle_pipe(current_cmd, current_cmd->next);
			pids[i] = fork();
			if (pids[i] == -1)
			{
				perror("fork");
				exit(1);
			}
			if (pids[i] == 0)
			{
				io_handle(current_cmd);
				builtin_control(current_cmd, envp, token_list, pids, vars);
				exit(current_cmd->return_value);
			}
			if (current_cmd->output_type == IO_PIPE_OUT)
				close(current_cmd->output_fd);
			if (current_cmd->input_type == IO_PIPE_IN)
				close(current_cmd->input_fd);
			i++;
		}
		last_cmd = current_cmd;
		current_cmd = current_cmd->next;
	}

	i = 0;
	while (i < cmd_count)
	{
		// Only wait for processes that were actually created
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					write(STDERR_FILENO, "Quit (core dumped)\n", 19);
				else if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				
				// Make sure last_cmd is valid before using
				if (last_cmd && i == cmd_count - 1)
					last_cmd->return_value = 128 + WTERMSIG(status);
			}
			else if (WIFEXITED(status) && last_cmd && i == cmd_count - 1)
				last_cmd->return_value = WEXITSTATUS(status);
		}
		i++;
	}
	free(pids);
}

//echo "test 42 minishell" | cat | grep "test" | cat | cat | grep "42" | cat | cat | grep "minishell" | cat
//echo "42 minishell Ayse Sude" | tr 'a-z' 'A-Z' | tr ' ' '\n' | sort | uniq | rev | tr 'A-Z' 'a-z' | cat | cat | wc -l | cat
