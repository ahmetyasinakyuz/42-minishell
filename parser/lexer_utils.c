/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 06:50:01 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/21 18:33:06 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// stuctin iceriklerini ayarlamak icin kullanilan fonksiyon
t_lexer	*create_token(char *str, t_tokens token_type, int index)
{
	t_lexer	*new_token;

	new_token = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(str);
	new_token->token = token_type;
	new_token->i = index;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

// yeni bir token olusturup listeye ekler
void	add_token(t_lexer **list, t_lexer *new_token)
{
	t_lexer	*temp;

	if (!*list)
	{
		*list = new_token;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
	new_token->prev = temp;
}
