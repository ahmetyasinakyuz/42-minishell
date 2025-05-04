/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 14:29:22 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	shift_cmd_list(t_simple_cmds *cmd_list)
{
	int	i;

	i = 0;
	while (cmd_list->str[i + 1] != NULL)
	{
		cmd_list->str[i] = cmd_list->str[i + 1];
		i++;
	}
	cmd_list->str[i] = NULL;
}

void	shift_cmd_content(t_simple_cmds *cmd_list)
{
	int	i;

	i = 0;
	while (cmd_list->content[i + 1] != NULL)
	{
		cmd_list->content[i] = cmd_list->content[i + 1];
		i++;
	}
	cmd_list->content[i] = NULL;
}

void	normalize_cmd_list(t_simple_cmds *cmd_list, t_free *free_struct)
{
	if (!cmd_list->str || !cmd_list->str[0] || cmd_list->str[0][0] == '\0')
	{
		if (cmd_list->str && cmd_list->str[1])
			shift_cmd_list(cmd_list);
		if (cmd_list->content && cmd_list->content[1])
			shift_cmd_content(cmd_list);
		if (cmd_list->str && cmd_list->str[0] && cmd_list->str[0][0] != '\0')
			builtin_control(cmd_list, free_struct);
		else
			exit(0);
	}
}

void	builtin_control(t_simple_cmds *cmd_list, t_free *free_struct)
{
	normalize_cmd_list(cmd_list, free_struct);
	if (ft_strncmp("exit", cmd_list->str[0], 5) == 0)
	{
		free_struct->cmd_list = cmd_list;
		exit_builtin(cmd_list, free_struct);
		exit(cmd_list->return_value);
	}
	else if (ft_strncmp("echo", cmd_list->str[0], 5) == 0)
		echo_builtin(cmd_list);
	else if (ft_strncmp("pwd", cmd_list->str[0], 4) == 0)
		pwd_builtin(cmd_list);
	else if (ft_strncmp("env", cmd_list->str[0], 4) == 0)
		env_builtin(cmd_list, free_struct->envp);
	else
		none_built_in(cmd_list, free_struct->envp);
	exit(cmd_list->return_value);
}
