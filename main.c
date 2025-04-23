/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:39 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/23 16:12:48 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	run_shell(t_vars **vars, char **envp)
{
	char	*input;

	setup_signals();
	while (1)
	{
		reset_signal_handling();
		input = readline("bash$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (ft_strncmp(input, "exit", 5) == 0)
		{
			free(input);
			break ;
		}
		if (ft_strlen(input) > 0)
			add_history(input);
		parser(input, vars, envp);
		free(input);
	}
	rl_clear_history();
	clear_vars(vars);
}

int	main(int argc, char **argv, char **envp)
{
	t_vars	*vars;

	vars = NULL;
	(void)argv;
	if (argc == 1)
		run_shell(&vars, envp);
	else
		printf("Usage: ./minishell\n");
	return (0);
}

//valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell