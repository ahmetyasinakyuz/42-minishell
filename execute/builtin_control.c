/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/04/28 14:18:06 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void builtin_control(t_simple_cmds *cmd_list, char **envp, t_lexer *token_list, pid_t *pids, t_vars **vars)
{
	t_simple_cmds	*current_cmd;

	current_cmd = cmd_list;
	if(ft_strncmp("echo", *current_cmd->str, 5) == 0)
		echo_builtin(current_cmd);
	// if(ft_strncmp("pwd", *current_cmd->str, 4) == 0
	// 	&& !(*current_cmd->flag))
	// 	//pwd_builtin(current_cmd);
	// if(ft_strncmp("export", *current_cmd->str, 7) == 0
	// 	&& !(*current_cmd->flag))
	// 	//export_builtin(current_cmd);
	// if(ft_strncmp("unset", *current_cmd->str, 6) == 0
	// 	&& !(*current_cmd->flag))
	// 	//unset_builtin(current_cmd);
	// if(ft_strncmp("env", *current_cmd->str, 4) == 0
	// 	&& !(*current_cmd->flag))
	// 	//env_builtin(current_cmd);
	// if(ft_strncmp("exit", *current_cmd->str, 5) == 0
	// 	&& !(*current_cmd->flag))
	// 	//exit_builtin(current_cmd);
	// // none_built_in(current_cmd);
	else
		none_built_in(current_cmd, envp);
	free_command_list(cmd_list);
	free_lexer_list(token_list);
	free(pids);
	clear_vars(vars);
	exit(0);
}