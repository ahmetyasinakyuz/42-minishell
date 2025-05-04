/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmtemel <ahmtemel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:09:59 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 00:36:41 by ahmtemel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if(str[0] == '+' || str[0] == '-')
		i++;
	while(str[i])
	{
		if(ft_isdigit(str[i]) == 0)
			return(0);
		i++;
	}
	return (1);
}

void	exit_builtin(t_simple_cmds *cmd_list, char **envp, t_lexer *token_list, pid_t *pids, t_vars **vars)
{
	if(!(cmd_list->prev) && !(cmd_list->next))
	{
		int	i;
		int	flag;
		
		i = 0;
		flag = 0;
		if(cmd_list->content[1] && cmd_list->content[2])
		{
			write(2, "minishell: exit: too many arguments\n", 36);
			cmd_list->return_value = 1;
			return;
		}
		if(cmd_list->content[1])
		{
			if(ft_isnum(cmd_list->content[1]))
				i = ft_new_atoi(cmd_list->content[1], &flag);
			else
				i = 400;
		}
		write(STDOUT_FILENO, "exit\n", 5);
		free_lexer_list(token_list);
		free(pids);
		clear_vars(vars);
		free_env(envp);
		rl_clear_history();
		if(i == 400 || flag == -1)
		{
			write(2, "minishell: exit: ", 17);
			write(2, cmd_list->content[1], ft_strlen(cmd_list->content[1]));
			write(2, ": numeric argument required\n", 28);
			free_command_list(cmd_list);
			exit(2);
		}
		else
		{
			free_command_list(cmd_list);
			exit(i);
		}
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

void	execute(t_simple_cmds *cmd_list, char ***envp, t_lexer *token_list, t_vars **vars)
{
	t_exec_state	st;

	st.cmd_count = 0;
	st.status = 0;
	st.last_cmd = cmd_list;
	st.current_cmd = cmd_list;

	while (st.current_cmd)
	{
		st.cmd_count++;
		st.last_cmd = st.current_cmd;
		st.current_cmd = st.current_cmd->next;
	}

	st.pids = malloc(sizeof(pid_t) * st.cmd_count);
	if (!st.pids)
	{
		perror("malloc");
		return;
	}
	for (st.i = 0; st.i < st.cmd_count; st.i++)
		st.pids[st.i] = -1;

	setup_execute_signals();

	st.current_cmd = cmd_list;
	st.i = 0;
	while (st.current_cmd)
	{
		if (!((st.current_cmd->pipe == 1 || st.current_cmd->prev) &&
			(ft_strncmp("unset",  *st.current_cmd->str, 6) == 0 ||
			ft_strncmp("cd",     *st.current_cmd->str, 3) == 0 ||
			ft_strncmp("exit",   *st.current_cmd->str, 5) == 0)))
		{
			if (st.current_cmd->pipe == 0 && !st.current_cmd->prev)
			{
				if (ft_strncmp("export", *st.current_cmd->str, 7) == 0)
				{
					if (st.current_cmd->content && st.current_cmd->content[1])
						export_builtin(st.current_cmd, envp, 1);
					else
						export_builtin(st.current_cmd, envp, 0);
				}
				else if(ft_strncmp("unset", *st.current_cmd->str, 6) == 0)
					unset_builtin(st.current_cmd, envp);
				else if(ft_strncmp("cd", *st.current_cmd->str, 3) == 0)
					cd_builtin(st.current_cmd, *envp);
				else if(ft_strncmp("exit", *st.current_cmd->str, 5) == 0)
					exit_builtin(st.current_cmd, *envp, token_list, st.pids, vars);
				else
				{
					if (st.current_cmd->next)
						handle_pipe(st.current_cmd, st.current_cmd->next);
					st.pids[st.i] = fork();
					if (st.pids[st.i] == -1)
					{
						perror("fork");
						exit(1);
					}
					if (st.pids[st.i] == 0)
					{
						setup_child_signals();
						io_handle(st.current_cmd);
						builtin_control(st.current_cmd, envp, token_list, st.pids, vars);
					}
					if (st.current_cmd->output_type == IO_PIPE_OUT)
						close(st.current_cmd->output_fd);
					if (st.current_cmd->input_type == IO_PIPE_IN)
						close(st.current_cmd->input_fd);
					st.i++;
				}
			}
			else
			{
				if (st.current_cmd->next)
					handle_pipe(st.current_cmd, st.current_cmd->next);
				st.pids[st.i] = fork();
				if (st.pids[st.i] == -1)
				{
					perror("fork");
					exit(1);
				}
				if (st.pids[st.i] == 0)
				{
					setup_child_signals();
					io_handle(st.current_cmd);
					builtin_control(st.current_cmd, envp, token_list, st.pids, vars);
				}
				if (st.current_cmd->output_type == IO_PIPE_OUT)
					close(st.current_cmd->output_fd);
				if (st.current_cmd->input_type == IO_PIPE_IN)
					close(st.current_cmd->input_fd);
				st.i++;
			}
		}
		st.last_cmd = st.current_cmd;
		st.current_cmd = st.current_cmd->next;
	} 

	st.i = 0;
	while (st.i < st.cmd_count)
	{
		if (st.pids[st.i] > 0)
		{
			waitpid(st.pids[st.i], &st.status, 0);
			if (WIFSIGNALED(st.status))
			{
				if (WTERMSIG(st.status) == SIGQUIT)
					write(STDERR_FILENO, "Quit (core dumped)\n", 19);
				else if (WTERMSIG(st.status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);

				if (st.last_cmd && st.i == st.cmd_count - 1)
					st.last_cmd->return_value = 128 + WTERMSIG(st.status);
			}
			else if (WIFEXITED(st.status) && st.last_cmd && st.i == st.cmd_count - 1)
				st.last_cmd->return_value = WEXITSTATUS(st.status);
		}
		st.i++;
	}

	setup_signals();
	free(st.pids);
}
