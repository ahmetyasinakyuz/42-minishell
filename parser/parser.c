/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/03/05 20:23:49 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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
		i = 0;
		while (current_cmd->flag && current_cmd->flag[i])
		{
			printf("flag: %s\n", current_cmd->flag[i]);
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

char	*is_dolar(char *str, t_vars **vars)
{
	int		i;
	char	*result;

	result = ft_strdup(str);
	free(str);
	i = 0;
	while (result[i])
	{
		if (result[i] == '$')
			result = handle_dollar_char(result, &i, vars);
		i++;
	}
	return (result);
}

void	handle_word_token(t_lexer *current, t_vars **vars)
{
	char	*str;

	str = current->str;
	if (str[0] == '\'')
		return ;
	found_var(str, vars);
	current->str = is_dolar(str, vars);
}

void	handle_current_token(t_lexer **current, t_lexer **start,
		t_simple_cmds **cmd_list, t_vars **vars)
{
	t_simple_cmds	*new_cmd;

	if ((*current)->token == WORD)
		handle_word_token(*current, vars);
	if ((*current)->token == PIPE)
	{
		new_cmd = create_command(*start, *current);
		if (new_cmd)
		{
			new_cmd->pipe = 1;
			add_command(cmd_list, new_cmd);
		}
		*start = (*current)->next;
	}
	else if (!(*current)->next)
	{
		new_cmd = create_command(*start, (*current)->next);
		if (new_cmd)
		{
			new_cmd->pipe = 0;
			add_command(cmd_list, new_cmd);
		}
	}
}

void	parse_commands(t_lexer *token_list, t_vars **vars)
{
	t_simple_cmds	*cmd_list;
	t_lexer			*start;
	t_lexer			*current;

	cmd_list = NULL;
	start = token_list;
	current = token_list;
	while (current)
	{
		handle_current_token(&current, &start, &cmd_list, vars);
		current = current->next;
	}
	print_cmd_list(cmd_list);
	free_command_list(cmd_list);
}

void	parser(char *input, t_vars **vars)
{
	t_lexer	*token_list;

	token_list = lexer(input);
	if (!token_list)
		return ;
	parse_commands(token_list, vars);
	free_lexer_list(token_list);
}
