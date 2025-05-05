/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 04:14:50 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_control(t_simple_cmds *cmd_list, char ***envp,
		t_lexer *token_list, pid_t *pids, t_vars **vars)
{
	t_simple_cmds	*current_cmd;
	t_exit_params	params;

	current_cmd = cmd_list;
	io_handle(current_cmd);
	
	// Prepare exit parameters
	params.cmd_list = cmd_list;
	params.envp = *envp;
	params.token_list = token_list;
	params.pids = pids;
	params.vars = vars;

	if (current_cmd->return_value != 0)
	{
		params.code = current_cmd->return_value;
		cleanup_and_exit(&params);
	}

	if (ft_strncmp("echo", *current_cmd->str, 5) == 0)
		echo_builtin(current_cmd);
	else if (ft_strncmp("export", *current_cmd->str, 7) == 0)
	{
		if (!current_cmd->content || !current_cmd->content[1])
			export_builtin(current_cmd, envp, 0);
		else
			export_builtin(current_cmd, envp, 1);
	}
	else if (ft_strncmp("env", *current_cmd->str, 4) == 0)
		env_builtin(cmd_list, envp);
	else if (ft_strncmp("pwd", *current_cmd->str, 4) == 0)
		pwd_builtin(current_cmd);
	else
		none_built_in(current_cmd, envp);
	
	params.code = current_cmd->return_value;
	cleanup_and_exit(&params);
}
