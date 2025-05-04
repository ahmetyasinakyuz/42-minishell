/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:09:59 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 12:18:31 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_all(t_free *free_struct)
{
	free_lexer_list(free_struct->token_list);
	free(free_struct->pids);
	clear_vars(free_struct->vars);
	free_env(free_struct->envp);
	rl_clear_history();
}

void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

void	exit_builtin_value(int i, int flag, t_simple_cmds *cmd_list)
{
	if (i == 400 || flag == -1)
	{
		printf("minishell: exit: %s: numeric argument required\n", cmd_list->content[1]);
		free_command_list(cmd_list);
		exit(2);
	}
	else
	{
		free_command_list(cmd_list);
		exit(i);
	}
}

void	exit_builtin(t_simple_cmds *cmd_list, t_free *free_struct)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	if (!(cmd_list->prev) && !(cmd_list->next))
	{
		if (cmd_list->content[1] && cmd_list->content[2])
		{
			printf("minishell: exit: too many arguments\n");
			cmd_list->return_value = 1;
			return ;
		}
		if (cmd_list->content[1])
		{
			if (ft_isnum(cmd_list->content[1]))
				i = ft_new_atoi(cmd_list->content[1], &flag);
			else
				i = 400;
		}
		write(STDOUT_FILENO, "exit\n", 5);
		free_all(free_struct);
		exit_builtin_value(i, flag, cmd_list);
	}
}

void	handle_pipe(t_simple_cmds *cmd, t_simple_cmds *next)
{
	int	pipe_fd[2];

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

void	count_commands(t_simple_cmds *cmd_list, int *cmd_count, t_simple_cmds **last_cmd)
{
	*cmd_count = 0;
	while (cmd_list)
	{
		(*cmd_count)++;
		*last_cmd = cmd_list;
		cmd_list = cmd_list->next;
	}
}

int	init_execute_struct(t_free *free_struct, pid_t **pids, int cmd_count,
						t_lexer *token_list, t_vars **vars, char ***envp)
{
	int	i;

	*pids = malloc(sizeof(pid_t) * cmd_count);
	if (!*pids)
	{
		perror("malloc");
		return (0);
	}
	i = 0;
	while (i < cmd_count)
		(*pids)[i++] = -1;
	free_struct->envp = *envp;
	free_struct->token_list = token_list;
	free_struct->pids = *pids;
	free_struct->vars = vars;
	return (1);
}

void	run_single_command(t_simple_cmds *cmd, t_free *free_struct, pid_t *pids, int i)
{
	if (cmd->next)
		handle_pipe(cmd, cmd->next);
	pids[i] = fork();
	if (pids[i] == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pids[i] == 0)
	{
		setup_child_signals();
		io_handle(cmd);
		builtin_control(cmd, free_struct);
	}
	if (cmd->output_type == IO_PIPE_OUT)
		close(cmd->output_fd);
	if (cmd->input_type == IO_PIPE_IN)
		close(cmd->input_fd);
}

void	wait_for_children(pid_t *pids, int cmd_count, t_simple_cmds *cmd_list)
{
	int	i;
	int	status;
	t_simple_cmds *current_cmd;

	i = 0;
	current_cmd = cmd_list;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					write(STDERR_FILENO, "Quit (core dumped)\n", 19);
				else if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				current_cmd->return_value = 128 + WTERMSIG(status);
			}
			else if (WIFEXITED(status))
				current_cmd->return_value = WEXITSTATUS(status);
		}
		if (current_cmd && current_cmd->next)
			current_cmd = current_cmd->next;
		i++;
	}
}

void	execute(t_simple_cmds *cmd_list, char ***envp,
	t_lexer *token_list, t_vars **vars)
{
	t_exec_state	state;
	
	count_commands(cmd_list, &state.cmd_count, &state.last_cmd);
	if (!init_execute_struct(&state.free_struct, &state.pids, state.cmd_count, token_list, vars, envp))
		return ;
	setup_execute_signals();
	state.free_struct.cmd_list = cmd_list;
	state.current_cmd = cmd_list;
	state.i = 0;
	while (state.current_cmd)
	{
		if (ft_strncmp("export", *state.current_cmd->str, 7) == 0)
			export_builtin(state.current_cmd, envp);
		else if (ft_strncmp("unset", *state.current_cmd->str, 6) == 0)
			unset_builtin(state.current_cmd, envp);
		else if (ft_strncmp("cd", *state.current_cmd->str, 3) == 0)
			cd_builtin(state.current_cmd, *envp);
		else if (ft_strncmp("echo", *state.current_cmd->str, 5) == 0 && 
				(!state.current_cmd->next && !state.current_cmd->prev))
		{
			echo_builtin(state.current_cmd);
		}
		else if (ft_strncmp("pwd", *state.current_cmd->str, 4) == 0 && 
				(!state.current_cmd->next && !state.current_cmd->prev))
		{
			pwd_builtin(state.current_cmd);
		}
		else if (ft_strncmp("env", *state.current_cmd->str, 4) == 0 && 
				(!state.current_cmd->next && !state.current_cmd->prev))
		{
			env_builtin(state.current_cmd, *envp);
		}
		else if (ft_strncmp("exit", *state.current_cmd->str, 5) == 0)
		{
			state.free_struct.envp = *envp;
			state.free_struct.token_list = token_list;
			state.free_struct.pids = state.pids;
			state.free_struct.vars = vars;
			exit_builtin(state.current_cmd, &state.free_struct);
		}
		else
			run_single_command(state.current_cmd, &state.free_struct, state.pids, state.i++);
		state.current_cmd = state.current_cmd->next;
	}
	wait_for_children(state.pids, state.i, cmd_list);
	setup_signals();
	free(state.pids);
}

//echo "test 42 minishell" | cat | grep "test" | cat | cat | grep "42" | cat | cat | grep "minishell" | cat
//echo "42 minishell Ayse Sude" | tr 'a-z' 'A-Z' | tr ' ' '\n' | sort | uniq | rev | tr 'A-Z' 'a-z' | cat | cat | wc -l | cat
// echo -n "This is a test" | cd /home/user | pwd | export | unset VAR_NAME | env | echo "Another test" | echo -n "Final output" | exit | echo "This should not be executed"
