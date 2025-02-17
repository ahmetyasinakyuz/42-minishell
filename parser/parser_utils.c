#include "parser.h"

int	check_quotes(char *input)
{
	int	i;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !double_quote)
			single_quote = !single_quote;
		if (input[i] == '\"' && !single_quote)
			double_quote = !double_quote;
		i++;
	}
	if (single_quote || double_quote)
	{
		printf("Error: unclosed quotes\n");
		return (0);
	}
	return (1);
}

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

t_simple_cmds	*create_command(t_lexer *start, t_lexer *end)
{
	t_simple_cmds	*cmd;
	t_lexer			*current;
	int				word_count;
	int				i;

	cmd = malloc(sizeof(t_simple_cmds));
	if (!cmd)
		return (NULL);
	word_count = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD)
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
		if (current->token == WORD)
			cmd->str[i++] = ft_strdup(current->str);
		current = current->next;
	}
	cmd->str[i] = NULL;
	cmd->builtin = NULL;
	cmd->num_redirections = 0;
	cmd->hd_file_name = NULL;
	cmd->redirections = NULL;
	cmd->next = NULL;
	cmd->prev = NULL;
	return (cmd);
}

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
