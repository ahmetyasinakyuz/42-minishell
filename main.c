/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:39 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/05 08:58:21 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_exit(t_vars **vars, char ***envp)
{
	rl_clear_history();
	clear_vars(vars);
	free_env(*envp);
}

int	is_input_incomplete(char *input)
{
	int	len;
	int	i;
	int	in_squote;
	int	in_dquote;

	len = ft_strlen(input);
	if (len == 0)
		return (0);
	i = len - 1;
	while (i >= 0 && (input[i] == ' ' || input[i] == '\t'))
		i--;
	if (i < 0)
		return (0);
	if (input[i] != '|')
		return (0);
	in_squote = 0;
	in_dquote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (input[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		i++;
	}
	return (!in_squote && !in_dquote);
}

char	*get_continuation(char *input)
{
	char	*continuation;
	char	*combined;
	char	*temp;

	continuation = readline("> ");
	if (!continuation)
		return (input);
	
	if (ft_strlen(continuation) > 0)
	{
		temp = ft_strjoin(input, " ");
		if (!temp)
		{
			free(continuation);
			return (input);
		}
		combined = ft_strjoin(temp, continuation);
		free(temp);
		free(input);
		free(continuation);
		if (!combined)
			return (NULL);
		return (combined);
	}
	free(continuation);
	return (input);
}

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
			{
				clean_exit(vars, envp);
				exit(130);
			}
			ft_putendl_fd("exit", STDOUT_FILENO);
			break ;
		}
		if (ft_strlen(input) > 0)
		{
			add_history(input);
			while (is_input_incomplete(input))
			{
				input = get_continuation(input);
				if (!input)
					break;
				if (ft_strlen(input) > 0)
					add_history(input);
			}
			if (input)
				parser(input, vars, envp);
			
		}
		if (input)
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
