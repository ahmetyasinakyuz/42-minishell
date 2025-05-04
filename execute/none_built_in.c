/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   none_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 02:26:32 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_direct_path(char *str)
{
	if (str[0] == '/')
		return (1);
	if (str[0] == '.' && str[1] == '/')
		return (1);
	if (str[0] == '.' && str[1] == '.' && str[2] == '/')
		return (1);
	return (0);
}

char	*resolve_command_path(t_simple_cmds *cmd_list, char **envp,
	int *allocated)
{
	char	*path;

	*allocated = 0;
	if (is_direct_path(cmd_list->str[0]))
	{
		if (access(cmd_list->str[0], F_OK) != 0)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_list->str[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			cmd_list->return_value = 127;
			return (NULL);
		}
		return (cmd_list->str[0]);
	}
	path = path_finder(cmd_list->str[0], envp);
	if (!path)
	{
		print_cmd_not_found(cmd_list->str[0], &cmd_list->return_value);
		return (NULL);
	}
	*allocated = 1;
	return (path);
}

void	handle_child_execution(t_simple_cmds *cmd_list, char *path,
	char ***envp)
{
	char	**cmd;

	if (cmd_list->flag == NULL)
		cmd = cmd_list->str;
	else
		cmd = merge_cmd_and_flags(cmd_list->str, cmd_list->flag);
	setup_child_signals();
	execve(path, cmd, *envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	execute_command(t_simple_cmds *cmd_list, char *path, char ***envp)
{
	pid_t	child_pid;
	int		status;

	child_pid = fork();
	if (child_pid == 0)
	{
		handle_child_execution(cmd_list, path, envp);
	}
	else if (child_pid > 0)
	{
		waitpid(child_pid, &status, 0);
		if (WIFEXITED(status))
			cmd_list->return_value = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			cmd_list->return_value = 128 + WTERMSIG(status);
		else
			cmd_list->return_value = 1;
	}
	else
	{
		perror("fork");
		cmd_list->return_value = 1;
	}
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
