/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   none_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/11 13:34:50 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_child_process(char **cmd, char *path, char ***envp,
	int cmd_allocated)
{
	setup_child_signals();
	execve(path, cmd, *envp);
	perror("execve");
	if (cmd_allocated)
		free(cmd);
	exit(EXIT_FAILURE);
}

static void	handle_parent_process(pid_t child_pid, t_simple_cmds *cmd_list,
	struct sigaction *old_int, struct sigaction *old_quit)
{
	int	status;

	waitpid(child_pid, &status, 0);
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(STDOUT_FILENO, "\n", 1);  // Always write newline for SIGINT
		else if (WTERMSIG(status) == SIGQUIT)
			write(STDERR_FILENO, "Quit (core dumped)\n", 19);
		cmd_list->return_value = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		cmd_list->return_value = WEXITSTATUS(status);
	else
		cmd_list->return_value = 1;
}

static void	handle_fork_error(t_simple_cmds *cmd_list,
	struct sigaction *old_int, struct sigaction *old_quit)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
	perror("fork");
	cmd_list->return_value = 1;
}

void	execute_command(t_simple_cmds *cmd_list, char *path, char ***envp)
{
	pid_t				child_pid;
	struct sigaction	old_int;
	struct sigaction	old_quit;
	char				**cmd;
	int					cmd_allocated;

	cmd = prepare_cmd(cmd_list, &cmd_allocated);
	sigaction(SIGINT, NULL, &old_int);
	sigaction(SIGQUIT, NULL, &old_quit);
	setup_execute_signals();
	child_pid = fork();
	if (child_pid == 0)
		handle_child_process(cmd, path, envp, cmd_allocated);
	else if (child_pid > 0)
		handle_parent_process(child_pid, cmd_list, &old_int, &old_quit);
	else
		handle_fork_error(cmd_list, &old_int, &old_quit);
	if (cmd_allocated)
		free(cmd);
}

void	none_built_in(t_simple_cmds *cmd_list, char ***envp)
{
	char	*path;
	int		path_allocated;

	if (is_invalid_command(cmd_list))
	{
		cmd_list->return_value = 0;
		return ;
	}
	path = resolve_command_path(cmd_list, *envp, &path_allocated);
	if (!path)
		return ;
	handle_command_error(cmd_list, cmd_list->str[0], path);
	if (cmd_list->return_value != 0)
	{
		if (path_allocated)
			free(path);
		return ;
	}
	execute_command(cmd_list, path, envp);
	if (path_allocated)
		free(path);
}
