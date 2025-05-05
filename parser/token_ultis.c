/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_ultis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:15:47 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 09:15:49 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_token(t_lexer **list, t_lexer *token)
{
	if (token->prev)
		token->prev->next = token->next;
	else
		*list = token->next;
	if (token->next)
		token->next->prev = token->prev;
	token->next = NULL;
	token->prev = NULL;
}

t_lexer	*copy_token(t_lexer *token)
{
	t_lexer	*new_token;

	new_token = malloc(sizeof(t_lexer));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(token->str);
	new_token->token = token->token;
	new_token->i = token->i;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}
