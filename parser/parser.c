/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/04 12:51:08 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*process_dollar(char *result, int *i, t_vars **vars)
{
	if (result[*i + 1] && (ft_isalpha(result[*i + 1]) || result[*i + 1] == '_'
			|| (ft_isdigit(result[*i + 1]) && result[*i + 1] != ' ')
			|| result[*i + 1] == '?'))
	{
		result = replace_env_var(result, *i, vars);
		*i = -1;
	}
	else
		(*i)++;
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
		handle_pipe_token(current, start, cmd_list, **vars);
	else if (!(*current)->next)
		handle_last_token(start, (*current)->next, cmd_list, **vars);
}

void	parse_commands(t_lexer *token_list, t_vars **vars, char ***envp)
{
	t_simple_cmds	*cmd_list;
	t_simple_cmds	*cmd_start;
	t_simple_cmds	*last_cmd;
	t_lexer			*start;
	t_lexer			*current;
	char			*return_value;

	cmd_list = NULL;
	start = token_list;
	current = token_list;
	while (current)
	{
		handle_current_token(&current, &start, &cmd_list, vars);
		current = current->next;
	}
	cmd_start = cmd_list;
	execute(cmd_list, envp, token_list, vars);
	last_cmd = cmd_start;
	while (last_cmd && last_cmd->next)
		last_cmd = last_cmd->next;
	if (last_cmd)
	{
		return_value = ft_itoa(last_cmd->return_value);
		add_static_var(vars, "?", return_value);
		free(return_value);
	}
	free_command_list(cmd_start);
}

void	parser(char *input, t_vars **vars, char ***envp)
{
	t_lexer	*token_list;
	char	*process;

	add_static_var(vars, "0", "minishell");
	process = remove_whitespaces(input);
	token_list = lexer(process);
	if (process)
		free(process);
	if (!token_list)
		return ;
	parse_commands(token_list, vars, envp);
	free_lexer_list(token_list);
}
