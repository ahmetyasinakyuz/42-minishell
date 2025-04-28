/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   none_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/04/28 14:06:36 by codespace        ###   ########.fr       */
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


void none_built_in(t_simple_cmds *cmd_list, char **envp, t_lexer *token_list)
{
	char	*path;
	char	**cmd;

	if(cmd_list->str[0] == NULL)
	{
		cmd_list->return_value = 127;
		return ;
	}
	if (cmd_list->str[0][0] == '/' ||
		(cmd_list->str[0][0] == '.' && cmd_list->str[0][1] == '/') ||
		(cmd_list->str[0][0] == '.' && cmd_list->str[0][1] == '.' && cmd_list->str[0][2] == '/'))
	{
		path = cmd_list->str[0];
	}
	else
	{
		path = path_finder(cmd_list->str[0], envp);
	}
	if (path == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_list->str[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cmd_list->return_value = 127;
		return ;
	}
	if(cmd_list->flag == NULL)
		cmd = cmd_list->str;
	else
	{
		cmd = merge_cmd_and_flags(cmd_list->str, cmd_list->flag);
	}
	execve(path, cmd, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}
