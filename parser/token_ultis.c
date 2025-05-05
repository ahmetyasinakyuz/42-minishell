/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_ultis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:15:47 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 09:21:40 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
