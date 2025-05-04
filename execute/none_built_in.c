/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   none_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+           +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 17:33:04 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

char	**merge_cmd_and_flags(char **cmd, char **flags)
{
	int		i;
	int		j;
	char	**merged;

	i = 0;
	j = 0;
	while (cmd[i])
		i++;
	while (flags[j])
		j++;
	merged = malloc(sizeof(char *) * (i + j + 1));
	if (!merged)
		return (NULL);
	i = 0;
	j = 0;
	while (cmd[i])
	{
		merged[i] = cmd[i];
		i++;
	}
	while (flags[j])
	{
		merged[i] = flags[j];
		i++;
		j++;
	}
	merged[i] = NULL;
	return (merged);
}

int	is_directory(const char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0)
		return (S_ISDIR(st.st_mode));
	return (0);
}

void	handle_command_error(t_simple_cmds *cmd_list, char *cmd, char *path)
{
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		cmd_list->return_value = 127;
		return;
	}
	else if (is_directory(path))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		cmd_list->return_value = 126;
		return;
	}
	else if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		cmd_list->return_value = 126;
		return;
	}
}

void	none_built_in(t_simple_cmds *cmd_list, char ***envp)
{
	char	*path;
	char	**cmd;
	pid_t	child_pid;
	int		status;
	int		path_allocated;

	path_allocated = 0;

	if (cmd_list->str[0] == NULL)
	{
		cmd_list->return_value = 127;
		return;
	}
	
	if (ft_strchr(cmd_list->str[0], '='))
	{
		cmd_list->return_value = 0;
		return;
	}
	
	if (cmd_list->str[0][0] == '\0')
	{
		ft_putstr_fd("minishell: : command not found\n", 2);
		cmd_list->return_value = 127;
		return;
	}
	
	if (cmd_list->str[0][0] == '/' || (cmd_list->str[0][0] == '.' &&
		cmd_list->str[0][1] == '/') || (cmd_list->str[0][0] == '.' &&
		cmd_list->str[0][1] == '.' && cmd_list->str[0][2] == '/'))
	{
		path = cmd_list->str[0];
		if (access(path, F_OK) == 0)
		{
			handle_command_error(cmd_list, cmd_list->str[0], path);
			if (cmd_list->return_value != 0)
				return;
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_list->str[0], 2);
			ft_putstr_fd(": No such file or directory\n", 2);
			cmd_list->return_value = 127;
			return;
		}
	}
	else
	{
		path = path_finder(cmd_list->str[0], *envp);
		path_allocated = 1; // Mark that path was dynamically allocated
		if (path == NULL)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_list->str[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			cmd_list->return_value = 127;
			return;
		}
		handle_command_error(cmd_list, cmd_list->str[0], path);
		if (cmd_list->return_value != 0)
		{
			if (path_allocated)
				free(path);
			return;
		}
	}
	
	child_pid = fork();
	if (child_pid == 0)
	{
		if (cmd_list->flag == NULL)
			cmd = cmd_list->str;
		else
			cmd = merge_cmd_and_flags(cmd_list->str, cmd_list->flag);
		
		setup_child_signals();
		execve(path, cmd, *envp);
		perror("execve");
		exit(EXIT_FAILURE);
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
	
	if (path_allocated)
		free(path);
}
