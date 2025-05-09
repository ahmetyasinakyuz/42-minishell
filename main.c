/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aysesudecami <aysesudecami@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:39 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/10 01:38:59 by aysesudecam      ###   ########.fr       */
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

void	handle_input_loop(char *input, t_vars **vars, char ***envp)
{
	add_history(input);
	while (is_input_incomplete(input))
	{
		input = get_continuation(input);
		if (!input)
			break ;
		if (ft_strlen(input) > 0)
			add_history(input);
	}
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

char	**env_maker(char **envp)
{
	int		i;
	char	**local_envp;

	i = 0;
	while (envp[i])
		i++;
	local_envp = malloc(sizeof(char *) * (i + 1));
	if (!local_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		local_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	local_envp[i] = NULL;
	return (local_envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_vars	*vars;
	char	**local_envp;

	local_envp = env_maker(envp);
	vars = NULL;
	(void)argv;
	if (argc == 1)
		run_shell(&vars, &local_envp);
	else
		printf("Usage: ./minishell\n");
	return (0);
}
