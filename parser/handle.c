/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:14:44 by aakyuz            #+#    #+#             */
/*   Updated: 2025/03/06 09:15:36 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	handle_word_token(t_lexer *current, t_vars **vars)
{
	char	*str;

	str = current->str;
	if (str[0] == '\'')
		return ;
	found_var(str, vars);
	current->str = is_dolar(str, vars);
}

void	handle_pipe_token(t_lexer **current, t_lexer **start,
		t_simple_cmds **cmd_list)
{
	t_simple_cmds	*new_cmd;

	new_cmd = create_command(*start, *current);
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
		t_simple_cmds **cmd_list)
{
	t_simple_cmds	*new_cmd;

	new_cmd = create_command(*start, end);
	if (new_cmd)
	{
		new_cmd->pipe = 0;
		if (*cmd_list && (*cmd_list)->pipe)
		{
			new_cmd->input_type = IO_PIPE_IN;
			new_cmd->input_fd = -1;
		}
		add_command(cmd_list, new_cmd);
	}
}

char	*handle_non_alpha_dollar(char *result, int *i)
{
	int		j;
	char	*temp;

	j = *i;
	while (result[j] && !ft_isalpha(result[j]))
		j++;
	temp = ft_strdup(result);
	free(result);
	result = ft_strjoin(ft_substr(temp, 0, *i), temp + j);
	free(temp);
	*i = -1;
	return (result);
}
