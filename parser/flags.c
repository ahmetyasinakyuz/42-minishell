/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyuz <akyuz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:53:22 by akyuz             #+#    #+#             */
/*   Updated: 2025/02/24 23:53:31 by akyuz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_flag(char *str)
{
	return (str && str[0] == '-');
}

int	count_flags(t_lexer *start, t_lexer *end)
{
	int		flag_count;
	t_lexer	*current;

	flag_count = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && is_flag(current->str) && (!current->prev
				|| current->prev->token == WORD))
			flag_count++;
		current = current->next;
	}
	return (flag_count);
}

void	fill_flags(t_simple_cmds *cmd, t_lexer *start, t_lexer *end)
{
	int		i;
	t_lexer	*current;

	i = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && is_flag(current->str) && (!current->prev
				|| current->prev->token == WORD))
			cmd->flag[i++] = ft_strdup(current->str);
		current = current->next;
	}
	cmd->flag[i] = NULL;
}
