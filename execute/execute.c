/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:09:59 by aycami            #+#    #+#             */
/*   Updated: 2025/04/21 18:34:32 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute(t_simple_cmds *cmd_list)
{
	t_simple_cmds	*current_cmd;

	current_cmd = cmd_list;
	while(current_cmd->pipe == 1)
	{
		builtin_control(current_cmd);
		current_cmd = current_cmd->next;
	}
	builtin_control(current_cmd);

}
