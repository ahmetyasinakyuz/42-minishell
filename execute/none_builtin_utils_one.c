/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   none_builtin_utils_one.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 02:24:54 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 02:27:07 by aycami           ###   ########.fr       */
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
