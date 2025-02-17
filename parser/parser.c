/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/17 19:24:40 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	add_command(t_simple_cmds **cmd_list, t_simple_cmds *new_cmd)
{
    printf("Adding new command to command list\n");
	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		return ;
	}
	t_simple_cmds	*temp;
	temp = *cmd_list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_cmd;
	new_cmd->prev = temp;
}

void	parse_commands(t_lexer *token_list)
{
	t_simple_cmds	*cmd_list;
	t_lexer			*start;
	t_lexer			*current;

    printf("\n=== Starting Command Parsing ===\n");
	cmd_list = NULL;
	start = token_list;
	current = token_list;
	while (current)
	{
        printf("Processing token: '%s'\n", current->str);
		if (current->token == PIPE)
		{
            printf("Found PIPE token, creating new command\n");
			add_command(&cmd_list, create_command(start, current));
			start = current->next;
		}
		else if (current->next == NULL)
		{
            printf("Reached end of tokens, creating final command\n");
			add_command(&cmd_list, create_command(start, current->next));
		}
		current = current->next;
	}
    printf("=== Command Parsing Complete ===\n\n");
	free_command_list(cmd_list);
}

void	parser(char *input)
{
    printf("\n====== PARSER START ======\n");
    printf("Parsing input: '%s'\n", input);
	t_lexer	*token_list;

	token_list = lexer(input);
	if (token_list)
	{
        printf("Lexer successful, starting command parsing\n");
		parse_commands(token_list);
	}
    else
    {
        printf("Lexer failed\n");
    }
    printf("====== PARSER END ======\n\n");
}
