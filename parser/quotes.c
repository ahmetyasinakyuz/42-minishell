/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:01:57 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/22 13:47:01 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	handle_quoted_string(char *str, int *i, char quote)
{
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (!str[*i])
		return (QUOTE_ERROR);
	return (SUCCESS);
}

int	validate_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			if (handle_quoted_string(str, &i, '\'') == QUOTE_ERROR)
				return (QUOTE_ERROR);
		}
		else if (str[i] == '\"')
		{
			if (handle_quoted_string(str, &i, '\"') == QUOTE_ERROR)
				return (QUOTE_ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

int	check_special_chars(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' || str[i] == ';')
			return (1);
		i++;
	}
	return (0);
}

char	*extract_token(char *input, int *i)
{
	int		j;
	char	*token;
	char	quote;
	int		has_single_quote;

	j = 0;
	has_single_quote = 0;
	token = malloc(ft_strlen(input) + 2);
	if (!token)
		return (NULL);
	ft_memset(token, 0, ft_strlen(input) + 2);
	if (input[*i] == '\'')
	{
		has_single_quote = 1;
		token[j++] = input[(*i)++];
		while (input[*i] && input[*i] != '\'')
			token[j++] = input[(*i)++];
		if (input[*i] == '\'')
			token[j++] = input[(*i)++];
	}
	else
	{
		while (input[*i])
		{
			if (input[*i] == '\'' || input[*i] == '\"')
			{
				quote = input[*i];
				(*i)++;
				while (input[*i] && input[*i] != quote)
					token[j++] = input[(*i)++];
				if (input[*i])
					(*i)++;
			}
			else if (input[*i] == ' ' && !has_single_quote && j > 0)
				break ;
			else if (input[*i] != ' ' || has_single_quote)
				token[j++] = input[(*i)++];
			else
				(*i)++;
		}
	}
	return (token);
}
