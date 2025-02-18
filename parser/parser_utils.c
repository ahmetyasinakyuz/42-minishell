/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:32 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/18 17:44:34 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_command_list(t_simple_cmds *list)
{
	t_simple_cmds	*temp;
	int				i;

	while (list)
	{
		temp = list;
		list = list->next;
		i = 0;
		while (temp->str[i])
			free(temp->str[i++]);
		free(temp->str);
		if (temp->hd_file_name)
			free(temp->hd_file_name);
		if (temp->redirections)
			free_lexer_list(temp->redirections);
		free(temp);
	}
}

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

void	add_redirection(t_lexer **redirection_list, t_lexer *token)
{
	t_lexer	*temp;

	if (!*redirection_list)
	{
		*redirection_list = token;
		return ;
	}
	temp = *redirection_list;
	while (temp->next)
		temp = temp->next;
	temp->next = token;
	token->prev = temp;
}

int	is_redirection(t_tokens token)
{
	return (token == REDIRECT_IN || token == REDIRECT_OUT
		|| token == REDIRECT_APPEND || token == REDIRECT_HEREDOC);
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

void	handle_redirections(t_simple_cmds *cmd, t_lexer **token_list)
{
	t_lexer	*current;
	t_lexer	*next;

	current = *token_list;
	while (current && current->token != PIPE)
	{
		next = current->next;
		if (is_redirection(current->token))
		{
			cmd->num_redirections++;
			if (next && next->token == WORD)
			{
				add_redirection(&cmd->redirections, copy_token(current));
				add_redirection(&cmd->redirections, copy_token(next));
			}
		}
		current = next;
	}
}

t_simple_cmds	*create_command(t_lexer *start, t_lexer *end)
{
	t_simple_cmds	*cmd;
	t_lexer			*current;
	int				word_count;
	int				i;

	cmd = malloc(sizeof(t_simple_cmds));
	if (!cmd)
		return (NULL);
	cmd->num_redirections = 0;
	cmd->hd_file_name = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	cmd->pipe = 0;
	handle_redirections(cmd, &start);
	word_count = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && (!current->prev
				|| !is_redirection(current->prev->token)))
			word_count++;
		current = current->next;
	}
	cmd->str = malloc(sizeof(char *) * (word_count + 1));
	if (!cmd->str)
	{
		free(cmd);
		return (NULL);
	}
	i = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && (!current->prev
				|| !is_redirection(current->prev->token)))
			cmd->str[i++] = ft_strdup(current->str);
		current = current->next;
	}
	cmd->str[i] = NULL;
	return (cmd);
}
