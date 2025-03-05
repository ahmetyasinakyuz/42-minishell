/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:24 by aakyuz            #+#    #+#             */
/*   Updated: 2025/03/05 15:38:43 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

t_lexer	*lexer_tokenize(char *input)
{
	t_lexer		*token_list;
	char		*token;
	int			i;
	t_tokens	token_type;

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
		else if (token)
			free(token);
	}
	return (token_list);
}

t_lexer	*lexer_precheck(char *input)
{
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
	return ((t_lexer *)1);
}

t_lexer	*lexer(char *input)
{
	if (!lexer_precheck(input))
		return (NULL);
	return (lexer_tokenize(input));
}
