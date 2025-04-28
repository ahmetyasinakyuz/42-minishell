/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysesudecami <aysesudecami@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:09:59 by aycami            #+#    #+#             */
/*   Updated: 2025/04/28 14:42:11 by aysesudecam      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void handle_pipe(t_simple_cmds *cmd, t_simple_cmds *next)
{
	int pipe_fd[2];
	pipe(pipe_fd);
	cmd->output_fd = pipe_fd[1];
	next->output_fd = pipe_fd[0];
}

void	execute(t_simple_cmds *cmd_list, char **envp)
{
	t_simple_cmds	*current_cmd;

	current_cmd = cmd_list;
	while(current_cmd->pipe == 1)
	{
		handle_pipe(current_cmd, current_cmd->next);
		builtin_control(current_cmd, envp);
		current_cmd = current_cmd->next;
	}
	builtin_control(current_cmd, envp);
}
