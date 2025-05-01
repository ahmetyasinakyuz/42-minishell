/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyuz <akyuz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:14:44 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/01 12:08:44 by akyuz            ###   ########.fr       */
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
		add_command(cmd_list, new_cmd);
	}
}
