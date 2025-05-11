/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_ultis.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:15:47 by aycami            #+#    #+#             */
/*   Updated: 2025/05/11 11:11:03 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

static int	check_initial_pipe(t_lexer *token_list)
{
	if (token_list && token_list->token == PIPE)
	{
		printf("bash: syntax error near unexpected token `|'\n");
		return (2);
	}
	return (0);
}

static int	validate_redirection_token(t_lexer *current)
{
	if (!current->next || is_redirection(current->next->token)
		|| current->next->token == PIPE)
	{
		ft_putstr_fd("bash: syntax error near unexpected token", STDERR_FILENO);
		ft_putstr_fd("`newline'\n", STDERR_FILENO);
		return (2);
	}
	return (0);
}

static int	validate_pipe_token(t_lexer *current)
{
	if (!current->next || current->next->token == PIPE)
	{
		if (!current->next)
			return (0);
		printf("bash: syntax error near unexpected token `|'\n");
		return (2);
	}
	return (0);
}

int	validate_redirections(t_lexer *token_list)
{
	t_lexer	*current;
	int		ret;

	ret = check_initial_pipe(token_list);
	if (ret != 0)
		return (ret);
	current = token_list;
	while (current)
	{
		if (is_redirection(current->token))
		{
			ret = validate_redirection_token(current);
			if (ret != 0)
				return (ret);
		}
		else if (current->token == PIPE)
		{
			ret = validate_pipe_token(current);
			if (ret != 0)
				return (ret);
		}
		current = current->next;
	}
	return (0);
}
