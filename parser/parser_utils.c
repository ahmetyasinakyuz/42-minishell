/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:32 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/17 19:24:40 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	free_command_list(t_simple_cmds *list)
{
	printf("\n=== Freeing Command List ===\n");
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
	printf("Command list freed\n\n");
}

int	is_redirection(t_tokens token)
{
	return (token == REDIRECT_IN || token == REDIRECT_OUT
		|| token == REDIRECT_APPEND || token == REDIRECT_HEREDOC);
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

void	handle_redirections(t_simple_cmds *cmd, t_lexer **token_list)
{
	t_lexer	*current;
	t_lexer	*next;
	t_lexer	*temp;

	current = *token_list;
	printf("\n=== Processing Redirections ===\n");
	while (current && current->token != PIPE)
	{
		next = current->next;
		if (is_redirection(current->token))
		{
			printf("Found redirection: '%s'\n", current->str);
			cmd->num_redirections++;
			if (next && next->token == WORD)
			{
				printf("Redirection target: '%s'\n", next->str);
				temp = next->next;
				remove_token(token_list, current);
				remove_token(token_list, next);
				add_redirection(&cmd->redirections, current);
				add_redirection(&cmd->redirections, next);
				current = temp;
				continue ;
			}
		}
		current = next;
	}
	printf("Total redirections found: %d\n", cmd->num_redirections);
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
	cmd->builtin = NULL;
	cmd->num_redirections = 0;
	cmd->hd_file_name = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	printf("\n=== Creating New Command ===\n");
	handle_redirections(cmd, &start);
	printf("Counting words in command...\n");
	word_count = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD)
		{
			word_count++;
			printf("Word found: '%s'\n", current->str);
		}
		current = current->next;
	}
	printf("Total words in command: %d\n", word_count);
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
		if (current->token == WORD)
			cmd->str[i++] = ft_strdup(current->str);
		current = current->next;
	}
	cmd->str[i] = NULL;
	printf("=== Command Creation Complete ===\n\n");
	return (cmd);
}

