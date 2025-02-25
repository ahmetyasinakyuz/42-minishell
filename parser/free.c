/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 06:35:43 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/25 14:25:20 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_lexer_list(t_lexer *list)
{
	t_lexer	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		free(temp->str);
		free(temp);
	}
}

void	free_command_list(t_simple_cmds *list)
{
	t_simple_cmds	*temp;
	int				i;

	while (list)
	{
		temp = list;
		list = list->next;
		i = 0;
		while (temp->str[i])
			free(temp->str[i++]);
		free(temp->str);
		if (temp->hd_file_name)
			free(temp->hd_file_name);
		if (temp->redirections)
			free_lexer_list(temp->redirections);
		free(temp);
	}
}

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
