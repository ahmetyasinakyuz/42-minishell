/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:14:03 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 09:14:07 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	exec_fork(t_handle_ctx *ctx, t_simple_cmds *cmd)
{
	t_fork_exec_ctx	fork_ctx;

	fork_ctx.cmd_list = cmd;
	fork_ctx.envp = ctx->envp;
	fork_ctx.token_list = ctx->token_list;
	fork_ctx.vars = ctx->vars;
	fork_and_execute(ctx->st, &fork_ctx);
}

static void	exec_exit(t_handle_ctx *ctx, t_simple_cmds *cmd)
{
	t_exitb_ctx	exit_ctx;

	exit_ctx.envp = *(ctx->envp);
	exit_ctx.token_list = ctx->token_list;
	exit_ctx.pids = ctx->st->pids;
	exit_ctx.vars = ctx->vars;
	exit_builtin(cmd, &exit_ctx);
}

static void	process_cmd(t_handle_ctx *ctx, t_simple_cmds *cmd)
{
	char	*cmd_str;

	cmd_str = *cmd->str;
	if (ft_strncmp("export", cmd_str, 7) == 0)
		export_builtin(cmd, ctx->envp, cmd->content && cmd->content[1]);
	else if (ft_strncmp("unset", cmd_str, 6) == 0)
		unset_builtin(cmd, ctx->envp);
	else if (ft_strncmp("cd", cmd_str, 3) == 0)
		cd_builtin(cmd, *(ctx->envp));
	else if (ft_strncmp("exit", cmd_str, 5) == 0)
		exec_exit(ctx, cmd);
	else
		exec_fork(ctx, cmd);
}

void	handle_builtin(t_handle_ctx *ctx)
{
	t_simple_cmds	*cmd;

	cmd = ctx->st->current_cmd;
	if (cmd->pipe != 0 || cmd->prev)
		return (exec_fork(ctx, cmd));
	process_cmd(ctx, cmd);
}

void	execute(t_simple_cmds *cmd_list, char ***envp, t_lexer *token_list,
		t_vars **vars)
{
	t_exec_state	st;
	t_handle_ctx	ctx;

	init_exec_state(&st, cmd_list);
	ctx.st = &st;
	ctx.envp = envp;
	ctx.token_list = token_list;
	ctx.vars = vars;
	setup_execute_signals();
	st.current_cmd = cmd_list;
	st.i = 0;
	while (st.current_cmd)
	{
		if (!((st.current_cmd->pipe == 1 || st.current_cmd->prev)
				&& (ft_strncmp("unset", *st.current_cmd->str, 6) == 0
					|| ft_strncmp("cd", *st.current_cmd->str, 3) == 0
					|| ft_strncmp("exit", *st.current_cmd->str, 5) == 0)))
			handle_builtin(&ctx);
		st.last_cmd = st.current_cmd;
		st.current_cmd = st.current_cmd->next;
	}
	wait_for_children(&st);
	setup_signals();
	free(st.pids);
}
