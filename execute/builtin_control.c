/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/03 19:08:32 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_control(t_simple_cmds *cmd_list, t_free *free_struct)
{
	t_simple_cmds	*current_cmd;

	current_cmd = cmd_list;
	if (ft_strncmp("echo", *current_cmd->str, 5) == 0)
		echo_builtin(current_cmd);
	else if (ft_strncmp("env", *current_cmd->str, 4) == 0)
		env_builtin(cmd_list, free_struct->envp);
	else if (ft_strncmp("pwd", *current_cmd->str, 4) == 0)
		pwd_builtin(current_cmd);
	else
		none_built_in(current_cmd, free_struct->envp);
	free_command_list(cmd_list);
	free_all(free_struct);
	exit(0);
}
