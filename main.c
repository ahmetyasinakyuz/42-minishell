/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:39 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/20 01:51:54 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clear_vars(t_vars **vars)
{
	t_vars	*temp;

	while (*vars)
	{
		temp = *vars;
		*vars = (*vars)->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}

int	main(int argc, char **argv)
{
	char	*input;
	t_vars	*vars;

	vars = NULL;
	(void)argv;
	if (argc == 1)
	{
		while (1)
		{
			input = readline("minihell> ");
			if (!input)
				break ;
			if (ft_strncmp(input, "exit", 5) == 0)
			{
				free(input);
				break ;
			}
			if (ft_strlen(input) > 0)
				add_history(input);
			parser(input, vars);
			free(input);
		}
		rl_clear_history();
		clear_vars(&vars);
	}
	else
		printf("Usage: ./minishell\n");
}
