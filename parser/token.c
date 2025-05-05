/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:15:52 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 09:15:54 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pipe_char(char *token, char *input, int *i, int *j)
{
	if (*j > 0)
		return (1);
	token[(*j)++] = input[(*i)++];
	return (1);
}

int	handle_redirect_char(char *token, char *input, int *i, int *j)
{
	if (*j > 0)
		return (1);
	token[(*j)++] = input[(*i)++];
	if (input[*i] == '<' || input[*i] == '>')
		token[(*j)++] = input[(*i)++];
	return (1);
}

void	handle_quotes(char c, int *in_squote, int *in_dquote)
{
	if (c == '\'' && !*in_dquote)
		*in_squote = !*in_squote;
	else if (c == '\"' && !*in_squote)
		*in_dquote = !*in_dquote;
}

void	process_token_char(char *token, char *input, int *i, int *j)
{
	int	in_squote;
	int	in_dquote;

	in_squote = 0;
	in_dquote = 0;
	while (input[*i])
	{
		handle_quotes(input[*i], &in_squote, &in_dquote);
		if (input[*i] == '|' && !in_squote && !in_dquote)
			if (handle_pipe_char(token, input, i, j))
				break ;
		if ((input[*i] == '<' || input[*i] == '>') && !in_squote && !in_dquote)
			if (handle_redirect_char(token, input, i, j))
				break ;
		if ((input[*i] == ' ' && !in_squote && !in_dquote && *j > 0))
			break ;
		token[(*j)++] = input[(*i)++];
	}
}

char	*extract_token(char *input, int *i)
{
	int		j;
	char	*token;

	j = 0;
	while (input[*i] == ' ')
		(*i)++;
	token = ft_calloc(ft_strlen(input) + 1, sizeof(char));
	if (!token)
		return (NULL);
	process_token_char(token, input, i, &j);
	if (input[*i] == ' ')
		(*i)++;
	return (token);
}
