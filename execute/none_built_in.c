/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   none_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 16:00:56 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	zero_values(int i[2])
{
	i[0] = 0;
	i[1] = 0;
}

char	**merge_cmd_and_flags(char **cmd, char **flags)
{
	int		i[2];
	char	**merged;

	zero_values(i);
	while (cmd[i[0]])
		i[0]++;
	while (flags[i[1]])
		i[1]++;
	merged = malloc(sizeof(char *) * (i[0] + i[1] + 1));
	if (!merged)
		return (NULL);
	zero_values(i);
	while (cmd[i[0]])
	{
		merged[i[0]] = cmd[i[0]];
		i[0]++;
	}
	while (flags[i[1]])
	{
		merged[i[0]] = flags[i[1]];
		i[0]++;
		i[1]++;
	}
	merged[i[0]] = NULL;
	return (merged);
}

void	none_built_in_path_control(t_simple_cmds *cmd_list,
		char **envp, char **path)
{
	if (cmd_list->str[0] == NULL || cmd_list->str[0][0] == '\0')
	{
		cmd_list->return_value = 0;
		*path = NULL;
		return ;
	}
	if (ft_strchr(cmd_list->str[0], '='))
	{
		cmd_list->return_value = 0;
		*path = NULL;
		return ;
	}
	if (cmd_list->str[0][0] == '/' || (cmd_list->str[0][0] == '.'
			&& cmd_list->str[0][1] == '/') || (cmd_list->str[0][0] == '.'
			&& cmd_list->str[0][1] == '.' && cmd_list->str[0][2] == '/'))
		*path = ft_strdup(cmd_list->str[0]);  // Use ft_strdup to avoid modifying original
	else
		*path = path_finder(cmd_list->str[0], envp);
}

void	none_built_in(t_simple_cmds *cmd_list, char **envp, t_free *free_struct)
{
	char	*path;
	char	**cmd;
	struct stat	path_stat;
	int		return_value;

	path = NULL;
	none_built_in_path_control(cmd_list, envp, &path);
	if (path == NULL)
	{
		if (cmd_list->str[0] && cmd_list->str[0][0] != '\0')
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd_list->str[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			cmd_list->return_value = 127;
		}
		return_value = cmd_list->return_value;
		free_command_list(cmd_list);
		free_lexer_list(free_struct->token_list);
		free(free_struct->pids);
		clear_vars(free_struct->vars);
		free_env(free_struct->envp);
		exit(return_value);
	}
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		cmd_list->return_value = 127;
		return_value = cmd_list->return_value;
		free(path);
		free_command_list(cmd_list);
		free_lexer_list(free_struct->token_list);
		free(free_struct->pids);
		clear_vars(free_struct->vars);
		free_env(free_struct->envp);
		exit(return_value);
	}
	if (stat(path, &path_stat) == 0 && S_ISDIR(path_stat.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		cmd_list->return_value = 126;
		return_value = cmd_list->return_value;
		free(path);
		free_command_list(cmd_list);
		free_lexer_list(free_struct->token_list);
		free(free_struct->pids);
		clear_vars(free_struct->vars);
		free_env(free_struct->envp);
		exit(return_value);
	}
	if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		cmd_list->return_value = 126;
		return_value = cmd_list->return_value;
		free(path);
		free_command_list(cmd_list);
		free_lexer_list(free_struct->token_list);
		free(free_struct->pids);
		clear_vars(free_struct->vars);
		free_env(free_struct->envp);
		exit(return_value);
	}
	if (cmd_list->flag == NULL)
		cmd = cmd_list->str;
	else
		cmd = merge_cmd_and_flags(cmd_list->str, cmd_list->flag);
	setup_child_signals();
	execve(path, cmd, envp);
	perror("execve");
	free(path);
	exit(EXIT_FAILURE);
}
