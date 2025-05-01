/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyuz <akyuz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:01:57 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/01 12:02:38 by akyuz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_quoted_string(char *str, int *i, char quote)
{
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (!str[*i])
		return (1);
	return (0);
}

int	validate_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			if (handle_quoted_string(str, &i, '\'') == 1)
				return (1);
		}
		else if (str[i] == '\"')
		{
			if (handle_quoted_string(str, &i, '\"') == 1)
				return (1);
		}
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

void	process_quotes(char *str, char *result, int *i, int *j)
{
	int	in_squote;
	int	in_dquote;

	in_squote = 0;
	in_dquote = 0;
	while (str[*i])
	{
		if (str[*i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (str[*i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else
			result[(*j)++] = str[*i];
		(*i)++;
	}
	result[*j] = '\0';
}
