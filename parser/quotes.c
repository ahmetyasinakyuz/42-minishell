/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:01:57 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/28 16:00:11 by aakyuz           ###   ########.fr       */
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

// boşluğa göre split yerine quote'lara göre split yapar
static void	handle_quote_status(char *input, int *i, int *in_quotes,
		char *quote_char)
{
	if ((input[*i] == '\'' || input[*i] == '\"') && !(*in_quotes))
	{
		*in_quotes = 1;
		*quote_char = input[*i];
	}
	else if (input[*i] == *quote_char && *in_quotes)
	{
		*in_quotes = 0;
		*quote_char = 0;
	}
}

char	*extract_token(char *input, int *i)
{
	int		j;
	char	*token;
	int		in_quotes;
	char	quote_char;

	j = 0;
	while (input[*i] == ' ')
		(*i)++;
	token = ft_calloc(ft_strlen(input) + 1, sizeof(char));
	if (!token)
		return (NULL);
	in_quotes = 0;
	quote_char = 0;
	while (input[*i])
	{
		handle_quote_status(input, i, &in_quotes, &quote_char);
		if ((input[*i] == ' ' && !in_quotes && j > 0))
			break ;
		token[j++] = input[(*i)++];
	}
	if (input[*i] == ' ')
		(*i)++;
	return (token);
}
