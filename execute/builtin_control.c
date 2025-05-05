/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 04:50:25 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	cleanup_and_exit(t_simple_cmds *cmd, t_builtin_ctx *ctx, int status)
{
	free_command_list(cmd);
	free_lexer_list(ctx->token_list);
	free(ctx->pids);
	clear_vars(ctx->vars);
	free_env(*(ctx->envp));
	exit(status);
}

static void	execute_builtin(t_simple_cmds *cmd, t_builtin_ctx *ctx)
{
	if (ft_strncmp("echo", *cmd->str, 5) == 0)
		echo_builtin(cmd);
	else if (ft_strncmp("export", *cmd->str, 7) == 0)
	{
		if (!cmd->content || !cmd->content[1])
			export_builtin(cmd, ctx->envp, 0);
		else
			export_builtin(cmd, ctx->envp, 1);
	}
	else if (ft_strncmp("env", *cmd->str, 4) == 0)
		env_builtin(cmd, ctx->envp);
	else if (ft_strncmp("pwd", *cmd->str, 4) == 0)
		pwd_builtin(cmd);
	else
		none_built_in(cmd, ctx->envp);
}

void	builtin_control(t_simple_cmds *cmd_list, t_builtin_ctx *ctx)
{
	t_simple_cmds	*current_cmd;
	int				exit_status;

	current_cmd = cmd_list;
	io_handle(current_cmd);
	if (current_cmd->return_value != 0)
		cleanup_and_exit(cmd_list, ctx, current_cmd->return_value);
	execute_builtin(current_cmd, ctx);
	exit_status = current_cmd->return_value;
	cleanup_and_exit(cmd_list, ctx, exit_status);
}
