/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 06:35:43 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/02 19:47:46 by aakyuz           ###   ########.fr       */
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

void	free_str_array(char **arr)
{
	int	i;

	if (!arr)
		return ;
	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}

void	free_command_content(t_simple_cmds *cmd)
{
	free_str_array(cmd->str);
	free_str_array(cmd->flag);
	free_str_array(cmd->content);
	if (cmd->hd_file_name)
	{
		unlink(cmd->hd_file_name);
		free(cmd->hd_file_name);
	}
	if (cmd->redirections)
		free_lexer_list(cmd->redirections);
}

void	free_command_list(t_simple_cmds *list)
{
	t_simple_cmds	*temp;

	while (list && list->prev)
		list = list->prev;
	while (list)
	{
		temp = list;
		list = list->next;
		free_command_content(temp);
		free(temp);
	}
}

void	clear_vars(t_vars **vars)
{
	t_vars	*temp;

	if (!vars || !*vars)
		return ;
	while (*vars)
	{
		temp = *vars;
		*vars = (*vars)->next;
		if (temp->key)
			free(temp->key);
		if (temp->value)
			free(temp->value);
		free(temp);
	}
	*vars = NULL;
}
