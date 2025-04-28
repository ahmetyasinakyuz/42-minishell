/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:35:39 by akyuz             #+#    #+#             */
/*   Updated: 2025/04/22 14:06:29 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_token(t_lexer **list, t_lexer *token)
{
	if (token->prev)
		token->prev->next = token->next;
	else
		*list = token->next;
	if (token->next)
		token->next->prev = token->prev;
	token->next = NULL;
	token->prev = NULL;
}

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

int	handle_pipe_char(char *token, char *input, int *i, int *j)
{
	if (*j > 0)
		return (1);
	token[(*j)++] = input[(*i)++];
	return (1);
}

void	process_token_char(char *token, char *input, int *i, int *j)
{
	int	in_squote;
	int	in_dquote;

	in_squote = 0;
	in_dquote = 0;
	while (input[*i])
	{
		// tek tırnak ve çift tırnak durumunu güncelle
		// && !in_dquote olmasinin sebebi tirnak ici tirnak oldugu zaman
		if (input[*i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (input[*i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		// eger pipe karakteri varsa ve tirnak icinde degilse
		// ve tokenin icinde hicbir karakter yoksa
		// pipe karakterini ekle ve break yap
		if (input[*i] == '|' && !in_squote && !in_dquote)
			if (handle_pipe_char(token, input, i, j))
				break;
		// heredoc ve yönlendirme karakterlerini boşluk olmadan algıla
		if ((input[*i] == '<' || input[*i] == '>') && !in_squote && !in_dquote)
		{
			if (*j > 0)
				break;
			token[(*j)++] = input[(*i)++];
			if (input[*i] == '<' || input[*i] == '>')
				token[(*j)++] = input[(*i)++];
			break;
		}
		// *j > 0 olmasinin sebebi, eger tokenin icinde hicbir karakter yoksa
		// ve ardinda bosluk varsa, boslugu ekleme
		if ((input[*i] == ' ' && !in_squote && !in_dquote && *j > 0))
			break;
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
