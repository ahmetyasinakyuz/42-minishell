/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   none_builtin_utils_one.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 02:24:54 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 08:32:02 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**allocate_merged_array(char **cmd, char **flags)
{
	int		cmd_len;
	int		flags_len;
	char	**merged;

	cmd_len = 0;
	flags_len = 0;
	while (cmd[cmd_len])
		cmd_len++;
	while (flags[flags_len])
		flags_len++;
	merged = malloc(sizeof(char *) * (cmd_len + flags_len + 1));
	return (merged);
}

char	**merge_cmd_and_flags(char **cmd, char **flags)
{
	char	**merged;
	int		i;
	int		j;

	merged = allocate_merged_array(cmd, flags);
	if (!merged)
		return (NULL);
	i = 0;
	while (cmd[i])
	{
		merged[i] = cmd[i];
		i++;
	}
	j = 0;
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
