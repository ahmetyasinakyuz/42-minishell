/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:14:44 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/11 10:55:02 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	handle_word_token(t_lexer *current, t_vars **vars)
{
	char	*str;
	t_lexer	*start;

	str = current->str;
	if (str[0] == '\'')
		return ;
	if (current->prev && current->prev->token == REDIRECT_HEREDOC)
		return ;
	if (ft_strchr(str, '=') && current->prev)
	{
		start = current;
		while (start->prev && start->prev->token != PIPE)
			start = start->prev;
		if (!start->str || ft_strncmp(start->str, "export", 7) != 0)
		{
			current->str = is_dolar(str, vars);
			return ;
		}
	}
	found_var(str, vars);
	current->str = is_dolar(str, vars);
}

void	handle_pipe_token(t_lexer **current, t_lexer **start,
		t_simple_cmds **cmd_list, t_vars vars)
{
	t_simple_cmds	*new_cmd;

	new_cmd = create_command(*start, *current, vars);
	if (new_cmd)
	{
		new_cmd->pipe = 1;
		new_cmd->output_type = IO_PIPE_OUT;
		new_cmd->output_fd = -1;
		add_command(cmd_list, new_cmd);
	}
	*start = (*current)->next;
}

void	handle_last_token(t_lexer **start, t_lexer *end,
		t_simple_cmds **cmd_list, t_vars vars)
{
	t_simple_cmds	*new_cmd;

	new_cmd = create_command(*start, end, vars);
	if (new_cmd)
	{
		new_cmd->pipe = 0;
		add_command(cmd_list, new_cmd);
	}
}

int	is_valid_var_char(char c)
{
	return (ft_isalpha(c) || c == '_' || (ft_isdigit(c) && c != ' '));
}

char	*process_dollar(char *result, int *i, t_vars **vars)
{
	if (result[*i + 1] == '$')
		return (process_dollar_sign(result, i, vars));
	if (result[*i + 1] == '?')
		return (process_question_mark(result, i, vars));
	if (result[*i + 1] && is_valid_var_char(result[*i + 1]))
	{
		result = replace_env_var(result, *i, vars);
		*i = -1;
	}
	else
		(*i)++;
	return (result);
}
