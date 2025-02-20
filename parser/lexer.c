/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:24 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/20 04:39:58 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

t_tokens	check_token_type(char *str)
{
	if (!str)
		return (WORD);
	if (ft_strncmp(str, "|", 2) == 0)
		return (PIPE);
	if (ft_strncmp(str, "<<", 3) == 0)
		return (REDIRECT_HEREDOC);
	if (ft_strncmp(str, "<", 2) == 0)
		return (REDIRECT_IN);
	if (ft_strncmp(str, ">>", 3) == 0)
		return (REDIRECT_APPEND);
	if (ft_strncmp(str, ">", 2) == 0)
		return (REDIRECT_OUT);
	return (WORD);
}

t_lexer	*lexer(char *input)
{
	t_lexer		*token_list;
	char		*token;
	int			i;
	t_tokens	token_type;

	if (validate_quotes(input) == QUOTE_ERROR)
	{
		printf("Error: Unclosed quotes\n");
		return (NULL);
	}
	if (check_special_chars(input))
	{
		printf("Error: Invalid special characters (\\, ;)\n");
		return (NULL);
	}
	token_list = NULL;
	i = 0;
	while (input[i])
	{
		token = extract_token(input, &i);
		if (token && *token)
		{
			token_type = check_token_type(token);
			add_token(&token_list, create_token(token, token_type, i));
			free(token);
		}
	}
	return (token_list);
}
