/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:01:57 by aakyuz            #+#    #+#             */
/*   Updated: 2025/03/05 16:15:25 by aakyuz           ###   ########.fr       */
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

void	handle_quote_status(char *input, int *i, int *in_squote, int *in_dquote)
{
	if (input[*i] == '\'' && !(*in_dquote))
		*in_squote = !(*in_squote);
	else if (input[*i] == '\"' && !(*in_squote))
		*in_dquote = !(*in_dquote);
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*result;
	int		in_squote;
	int		in_dquote;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	in_squote = 0;
	in_dquote = 0;
	while (str[i])
	{
		if (str[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (str[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	free(str);
	return (result);
}

char	*extract_token(char *input, int *i)
{
	int		j;
	char	*token;
	int		in_squote;
	int		in_dquote;

	j = 0;
	while (input[*i] == ' ')
		(*i)++;
	token = ft_calloc(ft_strlen(input) + 1, sizeof(char));
	if (!token)
		return (NULL);
	in_squote = 0;
	in_dquote = 0;
	while (input[*i])
	{
		if (input[*i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (input[*i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		if (input[*i] == '|' && !in_squote && !in_dquote)
		{
			if (j > 0)
				break ;
			token[j++] = input[(*i)++];
			break ;
		}
		if ((input[*i] == ' ' && !in_squote && !in_dquote && j > 0))
			break ;
		token[j++] = input[(*i)++];
	}
	if (input[*i] == ' ')
		(*i)++;
	return (token);
}
