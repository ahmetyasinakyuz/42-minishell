/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 05:50:47 by ahmtemel          #+#    #+#             */
/*   Updated: 2025/05/05 08:16:59 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_pipe(t_simple_cmds *cmd, t_simple_cmds *next)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	cmd->output_fd = pipe_fd[1];
	cmd->output_type = IO_PIPE_OUT;
	next->input_fd = pipe_fd[0];
	next->input_type = IO_PIPE_IN;
}

void	init_exec_state(t_exec_state *st, t_simple_cmds *cmd_list)
{
	int	i;

	i = 0;
	st->cmd_count = 0;
	st->status = 0;
	st->last_cmd = cmd_list;
	st->current_cmd = cmd_list;
	while (st->current_cmd)
	{
		st->cmd_count++;
		st->last_cmd = st->current_cmd;
		st->current_cmd = st->current_cmd->next;
	}
	st->pids = malloc(sizeof(pid_t) * st->cmd_count);
	if (!st->pids)
	{
		perror("malloc");
		exit(1);
	}
	while (i < st->cmd_count)
	{
		st->pids[i] = -1;
		i++;
	}
}

void	execute_child(t_simple_cmds *cmd, t_exec_child_ctx *ctx)
{
	t_builtin_ctx	builtin_ctx;

	builtin_ctx.envp = ctx->envp;
	builtin_ctx.token_list = ctx->token_list;
	builtin_ctx.pids = ctx->pids;
	builtin_ctx.vars = ctx->vars;
	setup_child_signals();
	io_handle(cmd);
	builtin_control(cmd, &builtin_ctx);
}

void	fork_and_execute(t_exec_state *st, t_fork_exec_ctx *ctx)
{
	t_exec_child_ctx	child_ctx;

	if (ctx->cmd_list->next)
		handle_pipe(ctx->cmd_list, ctx->cmd_list->next);
	st->pids[st->i] = fork();
	if (st->pids[st->i] == -1)
	{
		perror("fork");
		exit(1);
	}
	if (st->pids[st->i] == 0)
	{
		child_ctx.envp = ctx->envp;
		child_ctx.token_list = ctx->token_list;
		child_ctx.pids = st->pids;
		child_ctx.vars = ctx->vars;
		execute_child(ctx->cmd_list, &child_ctx);
	}
	if (ctx->cmd_list->output_type == IO_PIPE_OUT)
		close(ctx->cmd_list->output_fd);
	if (ctx->cmd_list->input_type == IO_PIPE_IN)
		close(ctx->cmd_list->input_fd);
	st->i++;
}

void	wait_for_children(t_exec_state *st)
{
	st->i = 0;
	while (st->i < st->cmd_count)
	{
		if (st->pids[st->i] > 0)
		{
			waitpid(st->pids[st->i], &st->status, 0);
			if (WIFSIGNALED(st->status))
			{
				if (WTERMSIG(st->status) == SIGQUIT)
					write(STDERR_FILENO, "Quit (core dumped)\n", 19);
				else if (WTERMSIG(st->status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				if (st->last_cmd && st->i == st->cmd_count - 1)
					st->last_cmd->return_value = 128 + WTERMSIG(st->status);
			}
			else if (WIFEXITED(st->status) && st->last_cmd
				&& st->i == st->cmd_count - 1)
				st->last_cmd->return_value = WEXITSTATUS(st->status);
		}
		st->i++;
	}
}
