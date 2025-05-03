/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:39 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/03 17:09:33 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//heredoc dollar
//sadece = komut bulunamadı hatası
//
// built in return value (echo, cd, pwd, export, unset, env, exit)
// norm
//
//- Try ctrl-C in a prompt after you wrote some stuff should show a new line with a new prompt
//- Try ctrl-D in a prompt after you wrote some stuff should not do anything
//- Try ctrl-\ in a prompt after you wrote some stuff should quit minishell --> RELAUNCH!
//


void	run_shell(t_vars **vars, char ***envp)
{
	char	*input;

	setup_signals();
	while (1)
	{
		reset_signal_handling();
		input = readline("bash$ ");
		if (!input)
		{
			if (g_received_signal == SIGINT)
				exit(130);
			break ;
		}
		if (ft_strlen(input) > 0)
			add_history(input);
		parser(input, vars, envp);
		free(input);
	}
	rl_clear_history();
	clear_vars(vars);
	free_env(*envp);
}

char **env_maker(char **envp)
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
	char **local_envp;

	local_envp = env_maker(envp);
	vars = NULL;
	(void)argv;
	if (argc == 1)
		run_shell(&vars, &local_envp);
	else
		printf("Usage: ./minishell\n");
	return (0);
}
