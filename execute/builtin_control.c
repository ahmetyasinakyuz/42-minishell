/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 12:28:40 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_control(t_simple_cmds *cmd_list, t_free *free_struct)
{
	// If no command or first element is empty string but there are more elements,
	// shift everything left
	if (!cmd_list->str || !cmd_list->str[0] || cmd_list->str[0][0] == '\0')
	{
		int i = 0;
		if (cmd_list->str && cmd_list->str[0] && cmd_list->str[1])
		{
			// Shift all strings to the left if first is empty
			while (cmd_list->str[i + 1] != NULL)
			{
				cmd_list->str[i] = cmd_list->str[i + 1];
				i++;
			}
			cmd_list->str[i] = NULL;
		}
		
		// If after shifting we have a command, execute it
		if (cmd_list->str && cmd_list->str[0] && cmd_list->str[0][0] != '\0')
			builtin_control(cmd_list, free_struct);
		else
			exit(0);
		return;
	}
	
	// Check for exit command
	if (ft_strncmp("exit", cmd_list->str[0], 5) == 0)
	{
		free_struct->cmd_list = cmd_list;
		exit_builtin(cmd_list, free_struct);
		// If we get here, exit was called with too many arguments
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
