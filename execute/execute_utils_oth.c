/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils_oth.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:13:52 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 09:13:54 by aycami           ###   ########.fr       */
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

void	cleanup(t_simple_cmds *cmd_list, t_cleanup_ctx *ctx)
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
	int				error_code;
	t_cleanup_ctx	cleanup_ctx;

	error_code = handle_exit_errors(cmd_list);
	if (error_code == 2)
	{
		write(2, "minishell: exit: numeric argument required\n", 43);
		cleanup_ctx.envp = ctx->envp;
		cleanup_ctx.token_list = ctx->token_list;
		cleanup_ctx.pids = ctx->pids;
		cleanup_ctx.vars = ctx->vars;
		cleanup_ctx.code = 2;
		cleanup(cmd_list, &cleanup_ctx);
	}
	cleanup_ctx.envp = ctx->envp;
	cleanup_ctx.token_list = ctx->token_list;
	cleanup_ctx.pids = ctx->pids;
	cleanup_ctx.vars = ctx->vars;
	cleanup_ctx.code = error_code;
	cleanup(cmd_list, &cleanup_ctx);
}
