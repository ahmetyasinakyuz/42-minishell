/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/19 23:32:44 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	add_command(t_simple_cmds **cmd_list, t_simple_cmds *new_cmd)
{
	t_simple_cmds	*temp;

	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		return ;
	}
	temp = *cmd_list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_cmd;
	new_cmd->prev = temp;
}

void	print_cmd_list(t_simple_cmds *cmd_list)
{
	t_simple_cmds	*current_cmd;
	t_lexer			*current_redir;
	int				i;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		printf("\n--- Command ---\n");
		printf("Pipe: %d\n", current_cmd->pipe);
		i = 0;
		while (current_cmd->str[i])
		{
			printf("cmd: %s\n", current_cmd->str[i]);
			i++;
		}
		current_redir = current_cmd->redirections;
		while (current_redir && current_redir->next)
		{
			printf("redirection type: ");
			if (current_redir->token == REDIRECT_IN)
				printf("REDIRECT_IN\n");
			else if (current_redir->token == REDIRECT_OUT)
				printf("REDIRECT_OUT\n");
			else if (current_redir->token == REDIRECT_APPEND)
				printf("REDIRECT_APPEND\n");
			else if (current_redir->token == REDIRECT_HEREDOC)
				printf("REDIRECT_HEREDOC\n");
			printf("redirection file: %s\n", current_redir->next->str);
			current_redir = current_redir->next->next;
		}
		printf("--------------\n");
		current_cmd = current_cmd->next;
	}
}

void	parse_commands(t_lexer *token_list)
{
	t_simple_cmds	*cmd_list;
	t_simple_cmds	*new_cmd;
	t_lexer			*start;
	t_lexer			*current;

	cmd_list = NULL;
	start = token_list;
	current = token_list;
	while (current)
	{
		if (current->token == WORD)
			current->str = is_dolar(current->str);
		if (current->token == PIPE)
		{
			new_cmd = create_command(start, current);
			new_cmd->pipe = 1;
			add_command(&cmd_list, new_cmd);
			start = current->next;
		}
		else if (current->next == NULL)
		{
			new_cmd = create_command(start, current->next);
			new_cmd->pipe = 0;
			add_command(&cmd_list, new_cmd);
		}
		current = current->next;
	}
	print_cmd_list(cmd_list);
	free_command_list(cmd_list);
}

void	parser(char *input, char ***vars)
{
	t_lexer	*token_list;

	(void)vars;
	token_list = lexer(input);
	if (!token_list)
		return ;
	parse_commands(token_list);
	free_lexer_list(token_list);
}
