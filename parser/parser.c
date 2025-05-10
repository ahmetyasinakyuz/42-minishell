/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/10 09:10:22 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	{
		handle_word_token(*current, vars);
		if ((*current)->str && (*current)->str[0] == '\0')
		{
			if (*current == *start)
				*start = (*current)->next;
		}
	}
	if ((*current)->token == PIPE)
		handle_pipe_token(current, start, cmd_list, **vars);
	else if (!(*current)->next)
		handle_last_token(start, (*current)->next, cmd_list, **vars);
}

void	update_return_value(t_simple_cmds *cmd_list, t_vars **vars)
{
	t_simple_cmds	*last_cmd;
	char			*return_value;

	last_cmd = cmd_list;
	while (last_cmd && last_cmd->next)
		last_cmd = last_cmd->next;
	if (last_cmd)
	{
		return_value = ft_itoa(last_cmd->return_value);
		add_static_var(vars, "?", return_value);
		free(return_value);
	}
}

void	parse_commands(t_lexer *token_list, t_vars **vars, char ***envp)
{
	t_simple_cmds	*cmd_list;
	t_lexer			*start;
	t_lexer			*current;
	int				validation_result;

	cmd_list = NULL;
	start = token_list;
	current = token_list;
	while (current)
	{
		handle_current_token(&current, &start, &cmd_list, vars);
		current = current->next;
	}
	validation_result = validate_redirections(token_list);
	if (validation_result == 0)
	{
		execute(cmd_list, envp, token_list, vars);
		update_return_value(cmd_list, vars);
	}
	else
		add_static_var(vars, "?", "2");
	free_command_list(cmd_list);
}

void	parser(char *input, t_vars **vars, char ***envp)
{
	t_lexer	*token_list;
	char	*process;

	add_static_var(vars, "0", "minishell");
	process = remove_whitespaces(input);
	if (input)
		free(input);
	token_list = lexer(process);
	if (process)
		free(process);
	if (!token_list)
		return ;
	parse_commands(token_list, vars, envp);
	free_lexer_list(token_list);
}
