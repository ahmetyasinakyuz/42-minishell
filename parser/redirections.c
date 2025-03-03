/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:18 by aakyuz            #+#    #+#             */
/*   Updated: 2025/03/03 09:54:36 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
				if (current->token == REDIRECT_IN)
					cmd->input_type = IO_FILE;
				else if (current->token == REDIRECT_HEREDOC)
					cmd->input_type = IO_HEREDOC;
				else if (current->token == REDIRECT_OUT
					|| current->token == REDIRECT_APPEND)
					cmd->output_type = IO_FILE;
				add_redirection(&cmd->redirections, copy_token(current));
				add_redirection(&cmd->redirections, copy_token(next));
			}
		}
		current = next;
	}
}

int	is_redirection(t_tokens token)
{
	return (token == REDIRECT_IN || token == REDIRECT_OUT
		|| token == REDIRECT_APPEND || token == REDIRECT_HEREDOC);
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
