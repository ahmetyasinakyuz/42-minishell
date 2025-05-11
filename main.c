/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmtemel <ahmtemel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:39 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/11 13:38:45 by ahmtemel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_null_input(t_vars **vars, char ***envp)
{
	if (g_received_signal == SIGINT)
	{
		clean_exit(vars, envp);
		exit(130);
	}
	ft_putendl_fd("exit", STDOUT_FILENO);
	return (1);
}

int	is_leading_pipe(char *input)
{
	int	i;

	i = 0;
	while (input[i] && (input[i] == ' ' || input[i] == '\t'))
		i++;
	if (input[i] == '|')
	{
		printf("bash: syntax error near unexpected token |'\n");
		return (1);
	}
	return (0);
}

int	handle_incomplete_input(char **input, t_vars **vars)
{
	while (is_input_incomplete(*input))
	{
		*input = get_continuation(*input);
		if (!*input)
		{
			add_static_var(vars, "?", "130");
			return (0);
		}
		if (ft_strlen(*input) > 0)
			add_history(*input);
	}
	return (1);
}

void	handle_input_loop(char *input, t_vars **vars, char ***envp)
{
	add_history(input);
	if (is_leading_pipe(input))
	{
		add_static_var(vars, "?", "2");
		free(input);
		return ;
	}
	if (!handle_incomplete_input(&input, vars))
		return ;
	if (input)
		parser(input, vars, envp);
}

void	run_shell(t_vars **vars, char ***envp)
{
	char	*input;

	setup_signals();
	while (1)
	{
		reset_signal_handling();
		input = readline("bash$ ");
		if (!input && handle_null_input(vars, envp))
			break ;
		if (g_received_signal == SIGINT)
		{
			add_static_var(vars, "?", "130");
			g_received_signal = 0;
		}
		if (ft_strlen(input) > 0)
			handle_input_loop(input, vars, envp);
		else if (input)
			free(input);
	}
	clean_exit(vars, envp);
}
