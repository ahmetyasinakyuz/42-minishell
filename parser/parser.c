/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/03/10 08:14:28 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	print_io_type(t_io_type type)
{
	if (type == IO_STDIN)
		printf("IO_STDIN");
	else if (type == IO_STDOUT)
		printf("IO_STDOUT");
	else if (type == IO_PIPE_IN)
		printf("IO_PIPE_IN");
	else if (type == IO_PIPE_OUT)
		printf("IO_PIPE_OUT");
	else if (type == IO_FILE_IN)
		printf("IO_FILE_IN");
	else if (type == IO_FILE_OUT)
		printf("IO_FILE_OUT");
	else if (type == IO_HEREDOC)
		printf("IO_HEREDOC");
	else if (type == IO_APPEND)
		printf("IO_APPEND");
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
		printf("Input type: ");
		print_io_type(current_cmd->input_type);
		printf(" (fd: %d)\n", current_cmd->input_fd);
		printf("Output type: ");
		print_io_type(current_cmd->output_type);
		printf(" (fd: %d)\n", current_cmd->output_fd);
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

static char	*process_dollar(char *result, int *i, t_vars **vars)
{
	if (result[*i + 1] && (ft_isalpha(result[*i + 1]) || ft_isdigit(result[*i + 1])))
	{
		result = replace_env_var(result, *i, vars);
		*i = -1;
	}
	else
		result = handle_non_alpha_dollar(result, i);
	return (result);
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
			result = process_dollar(result, &i, vars);
		i++;
	}
	return (result);
}

void	handle_current_token(t_lexer **current, t_lexer **start,
		t_simple_cmds **cmd_list, t_vars **vars)
{
	if ((*current)->token == WORD)
		handle_word_token(*current, vars);
	if ((*current)->token == PIPE)
		handle_pipe_token(current, start, cmd_list);
	else if (!(*current)->next)
		handle_last_token(start, (*current)->next, cmd_list);
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

	add_static_var(vars, "0", "minishell");
	token_list = lexer(input);
	if (!token_list)
		return ;
	parse_commands(token_list, vars);
	free_lexer_list(token_list);
}
