/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 06:50:01 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/03 15:00:12 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
}

static void	process_whitespace(char *str, char *result, int *i, int *j)
{
	int	in_quotes;

	in_quotes = 0;
	while (str[*i])
	{
		if (str[*i] == '"' || str[*i] == '\'')
			in_quotes = !in_quotes;
		if (!in_quotes && is_whitespace(str[*i]))
		{
			if (*j > 0 && result[*j - 1] != ' ')
				result[(*j)++] = ' ';
		}
		else
			result[(*j)++] = str[*i];
		(*i)++;
	}
}

char	*remove_whitespaces(char *str)
{
	char	*result;
	int		i;
	int		j;

	result = malloc(ft_strlen(str) + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	process_whitespace(str, result, &i, &j);
	if (j > 0 && result[j - 1] == ' ')
		j--;
	result[j] = '\0';
	return (result);
}