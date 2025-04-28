/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 06:35:43 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/28 13:23:15 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
		while (temp->str && temp->str[i])
			free(temp->str[i++]);
		if (temp->str)
			free(temp->str);
		i = 0;
		while (temp->flag && temp->flag[i])
			free(temp->flag[i++]);
		if (temp->flag)
			free(temp->flag);
		i = 0;
		while (temp->content && temp->content[i])
			free(temp->content[i++]);
		if (temp->content)
			free(temp->content);
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
