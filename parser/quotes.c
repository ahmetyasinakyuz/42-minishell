/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:01:57 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/10 20:20:43 by aakyuz           ###   ########.fr       */
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

void	process_quotes(char *str, char *result, int *i, int *j)
{
	int	in_squote;
	int	in_dquote;
	int	len;
	
	in_squote = 0;
	in_dquote = 0;
	len = ft_strlen(str);
	if (len == 2 && ((str[0] == '\'' && str[1] == '\'') || 
		(str[0] == '\"' && str[1] == '\"')))
	{
		result[0] = str[0];
		result[1] = str[1];
		result[2] = '\0';
		*i = 2;
		*j = 2;
		return;
	}
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
