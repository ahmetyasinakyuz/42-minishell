/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:39 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/19 13:43:32 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;

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
		parser(input);
		free(input);
	}
	rl_clear_history();
	return (0);
}
