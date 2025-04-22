/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:09:59 by aycami            #+#    #+#             */
/*   Updated: 2025/04/22 18:50:15 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	execute(t_simple_cmds *cmd_list, char **envp)
{
	t_simple_cmds	*current_cmd;

	(void)envp;
	current_cmd = cmd_list;
	while(current_cmd->pipe == 1)
	{
		builtin_control(current_cmd, envp);
		current_cmd = current_cmd->next;
	}
	builtin_control(current_cmd, envp);
}
