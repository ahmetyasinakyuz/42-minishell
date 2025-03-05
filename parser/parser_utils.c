/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:32 by aakyuz            #+#    #+#             */
/*   Updated: 2025/03/05 17:13:03 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	init_cmd(t_simple_cmds **cmd)
{
	*cmd = malloc(sizeof(t_simple_cmds));
	if (!*cmd)
		return (1);
	(*cmd)->num_redirections = 0;
	(*cmd)->hd_file_name = NULL;
	(*cmd)->redirections = NULL;
	(*cmd)->next = NULL;
	(*cmd)->prev = NULL;
	(*cmd)->pipe = 0;
	(*cmd)->str = NULL;
	(*cmd)->flag = NULL;
	return (0);
}

int	count_words(t_lexer *start, t_lexer *end)
{
	int		word_count;
	t_lexer	*current;

	word_count = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && (!current->prev
				|| !is_redirection(current->prev->token))
			&& !is_flag(current->str))
			word_count++;
		current = current->next;
	}
	return (word_count);
}

void	fill_words(t_simple_cmds *cmd, t_lexer *start, t_lexer *end)
{
	int		i;
	t_lexer	*current;
	char	*unquoted;
	char	*temp;

	i = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && (!current->prev
				|| !is_redirection(current->prev->token)))
		{
			temp = ft_strdup(current->str);
			if (!is_flag(temp))
			{
				unquoted = remove_quotes(temp);
				cmd->str[i++] = unquoted;
			}
			else
				free(temp);
		}
		current = current->next;
	}
	cmd->str[i] = NULL;
}
