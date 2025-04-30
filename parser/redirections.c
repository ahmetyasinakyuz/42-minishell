/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyuz <akyuz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:18 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/30 17:32:14 by akyuz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	update_io_type(t_simple_cmds *cmd, t_tokens token)
{
	if (token == REDIRECT_IN)
	{
		cmd->input_type = IO_FILE_IN;
		cmd->input_fd = -1;
	}
	else if (token == REDIRECT_OUT)
	{
		cmd->output_type = IO_FILE_OUT;
		cmd->output_fd = -1;
	}
	else if (token == REDIRECT_APPEND)
	{
		cmd->output_type = IO_APPEND;
		cmd->output_fd = -1;
	}
	else if (token == REDIRECT_HEREDOC)
	{
		cmd->input_type = IO_HEREDOC;
		cmd->input_fd = -1;
	}
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
			update_io_type(cmd, current->token);
			if (next && next->token == WORD)
			{
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

/// Bu fonksiyon, yönlendirme listesinin sonuna yeni bir yönlendirme ekler.
void	add_redirection(t_lexer **redirection_list, t_lexer *token)
{
	t_lexer	*temp;

	// Eğer yönlendirme listesi boşsa, yeni yönlendirmeyi başa ekle
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
