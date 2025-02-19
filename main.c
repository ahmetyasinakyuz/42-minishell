/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:39 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/19 23:31:07 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv)
{
	char	*input;
	char 	**vars;

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
			parser(input, &vars);
			free(input);
		}
		rl_clear_history();
	}
	else
		printf("Usage: ./minishell\n");
}
