#include "parser.h"

void	parse_commands(t_lexer *token_list)
{
	t_simple_cmds	*cmd_list;
	t_lexer			*start;
	t_lexer			*current;

	cmd_list = NULL;
	start = token_list;
	current = token_list;
	while (current)
	{
		if (current->token == PIPE)
		{
			add_command(&cmd_list, create_command(start, current));
			start = current->next;
		}
		else if (current->next == NULL)
		{
			add_command(&cmd_list, create_command(start, current->next));
		}
		current = current->next;
	}
	free_command_list(cmd_list);
}

void	parser(char *input)
{
	t_lexer	*token_list;

	if (!check_quotes(input))
		return ;
	token_list = lexer(input);
	if (token_list)
	{
		parse_commands(token_list);
		free_lexer_list(token_list);
	}
}
