/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:09:59 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 02:21:46 by aycami         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
		free(env[i++]);
	free(env);
}

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	handle_exit_errors(t_simple_cmds *cmd_list)
{
	int	flag;
	int	re;

	flag = 0;
	re = 0;
	if (cmd_list->content[1] && cmd_list->content[2])
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		return (1);
	}
	if (cmd_list->content[1])
	{
		if (!ft_isnum(cmd_list->content[1]))
			return (2);
		else
		{
			re = ft_new_atoi(cmd_list->content[1], &flag);
			if (flag == -1)
				return (2);
			return (re);
		}
	}
	return (0);
}

typedef struct s_cleanup_ctx
{
	char	**envp;
	t_lexer	*token_list;
	pid_t	*pids;
	t_vars	**vars;
	int		code;
}			t_cleanup_ctx;

void	cleanup_and_exit(t_simple_cmds *cmd_list, t_cleanup_ctx *ctx)
{
	write(STDOUT_FILENO, "exit\n", 5);
	free_lexer_list(ctx->token_list);
	free(ctx->pids);
	clear_vars(ctx->vars);
	free_env(ctx->envp);
	rl_clear_history();
	free_command_list(cmd_list);
	exit(ctx->code);
}

void	exit_builtin(t_simple_cmds *cmd_list, t_exitb_ctx *ctx)
{
	int	error_code;

	error_code = handle_exit_errors(cmd_list);
	if (error_code == 2)
	{
		write(2, "minishell: exit: numeric argument required\n", 43);
		cleanup_and_exit(cmd_list, &((t_cleanup_ctx){ctx->envp, ctx->token_list,
				ctx->pids, ctx->vars, 2}));
	}
	cleanup_and_exit(cmd_list, &((t_cleanup_ctx){ctx->envp, ctx->token_list,
			ctx->pids, ctx->vars, error_code}));
}

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
	if (ctx->cmd_list->next)
		handle_pipe(ctx->cmd_list, ctx->cmd_list->next);
	st->pids[st->i] = fork();
	if (st->pids[st->i] == -1)
	{
		perror("fork");
		exit(1);
	}
	if (st->pids[st->i] == 0)
		execute_child(ctx->cmd_list, &((t_exec_child_ctx){ctx->envp,
				ctx->token_list, st->pids, ctx->vars}));
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

void	handle_builtin(t_exec_state *st, char ***envp, t_lexer *token_list,
		t_vars **vars)
{
	t_simple_cmds	*cmd;

	cmd = st->current_cmd;
	if (cmd->pipe == 0 && !cmd->prev)
	{
		if (ft_strncmp("export", *cmd->str, 7) == 0)
			export_builtin(cmd, envp, cmd->content && cmd->content[1]);
		else if (ft_strncmp("unset", *cmd->str, 6) == 0)
			unset_builtin(cmd, envp);
		else if (ft_strncmp("cd", *cmd->str, 3) == 0)
			cd_builtin(cmd, *envp);
		else if (ft_strncmp("exit", *cmd->str, 5) == 0)
			exit_builtin(cmd, &((t_exitb_ctx){*envp, token_list, st->pids,
					vars}));
		else
			fork_and_execute(st, &((t_fork_exec_ctx){cmd, envp, token_list,
					vars}));
	}
	else
		fork_and_execute(st, &((t_fork_exec_ctx){cmd, envp, token_list, vars}));
}

void	execute(t_simple_cmds *cmd_list, char ***envp, t_lexer *token_list,
		t_vars **vars)
{
	t_exec_state	st;

	init_exec_state(&st, cmd_list);
	setup_execute_signals();
	st.current_cmd = cmd_list;
	st.i = 0;
	while (st.current_cmd)
	{
		if (!((st.current_cmd->pipe == 1 || st.current_cmd->prev)
				&& (ft_strncmp("unset", *st.current_cmd->str, 6) == 0
					|| ft_strncmp("cd", *st.current_cmd->str, 3) == 0
					|| ft_strncmp("exit", *st.current_cmd->str, 5) == 0)))
			handle_builtin(&st, envp, token_list, vars);
		st.last_cmd = st.current_cmd;
		st.current_cmd = st.current_cmd->next;
	}
	wait_for_children(&st);
	setup_signals();
	free(st.pids);
}
