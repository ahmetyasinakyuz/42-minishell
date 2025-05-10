/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_ultis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:15:47 by aycami            #+#    #+#             */
/*   Updated: 2025/05/10 09:22:09 by aakyuz           ###   ########.fr       */
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

int	validate_redirections(t_lexer *token_list)
{
	t_lexer	*current;

	current = token_list;
	if (current && current->token == PIPE)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		return (2);
	}
	while (current)
	{
		if (is_redirection(current->token))
		{
			if (!current->next || is_redirection(current->next->token)
				|| current->next->token == PIPE)
			{
				printf("bash: syntax error near unexpected token");
				ft_putstr_fd("`newline'\n", STDERR_FILENO);
				return (2);
			}
		}
		else if (current->token == PIPE)
		{
			if (!current->next || current->next->token == PIPE)
			{
				if (!current->next)
					return (0);
				printf("bash: syntax error near unexpected token `|'\n");
				return (2);
			}
		}
		current = current->next;
	}
	return (0);
}
