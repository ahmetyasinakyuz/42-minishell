/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/04/21 15:36:07 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void builtin_control(t_simple_cmds *cmd_list,char **env)
{
	t_simple_cmds	*current_cmd;

	current_cmd = cmd_list;
	if(ft_strncmp("echo", *current_cmd->str, 5) == 0
		&& (ft_strncmp("-n", *current_cmd->flag[0], 3) == 0
		||	!(current_cmd->flag)))
		//echo_builtin();
	if(ft_strncmp("pwd", *current_cmd->str, 4) == 0
		&& !(*current_cmd->flag))
		//pwd_builtin();
	if(ft_strncmp("export", *current_cmd->str, 7) == 0
		&& !(*current_cmd->flag))
		//export_builtin();
	if(ft_strncmp("unset", *current_cmd->str, 6) == 0
		&& !(*current_cmd->flag))
		//unset_builtin();

	if(ft_strncmp("env", *current_cmd->str, 4) == 0
		&& !(*current_cmd->flag))
		//env_builtin();
	if(ft_strncmp("exit", *current_cmd->str, 5) == 0
		&& !(*current_cmd->flag))
		//exit_builtin();
    none_built_in(current_cmd);
}