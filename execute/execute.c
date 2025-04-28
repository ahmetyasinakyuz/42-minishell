/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:09:59 by aycami            #+#    #+#             */
/*   Updated: 2025/04/28 13:06:29 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void execute(t_simple_cmds *cmd_list, char **envp)
{
	t_simple_cmds *current_cmd;
	t_simple_cmds *last_cmd;
	pid_t *pids;
	int cmd_count = 0;
	int i;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		cmd_count++;
		current_cmd = current_cmd->next;
	}
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		perror("malloc");
		return;
	}
	current_cmd = cmd_list;
	i = 0;
	while (current_cmd)
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
			builtin_control(current_cmd, envp);
			exit(current_cmd->return_value);
		}
		if (current_cmd->output_type == IO_PIPE_OUT)
			close(current_cmd->output_fd);
		if (current_cmd->input_type == IO_PIPE_IN)
			close(current_cmd->input_fd);
		last_cmd = current_cmd;
		current_cmd = current_cmd->next;
		i++;
	}

	int status;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				last_cmd->return_value = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_cmd->return_value = 128 + WTERMSIG(status);
		}
		i++;
	}
	free(pids);
}

//echo "test 42 minishell" | cat | grep "test" | cat | cat | grep "42" | cat | cat | grep "minishell" | cat
