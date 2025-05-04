/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 01:46:20 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_control(t_simple_cmds *cmd_list, char ***envp,
		t_lexer *token_list, pid_t *pids, t_vars **vars)
{
	t_simple_cmds	*current_cmd;
	int				exit_status;

	current_cmd = cmd_list;
	io_handle(current_cmd);
	if (current_cmd->return_value != 0)
	{
		exit_status = current_cmd->return_value;
		free_command_list(cmd_list);
		free_lexer_list(token_list);
		free(pids);
		clear_vars(vars);
		free_env(*envp);
		exit(exit_status);
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
	
	exit_status = current_cmd->return_value;
	free_command_list(cmd_list);
	free_lexer_list(token_list);
	free(pids);
	clear_vars(vars);
	free_env(*envp);
	exit(exit_status);
}
