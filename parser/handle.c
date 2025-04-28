/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:14:44 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/22 14:09:51 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"


void	handle_word_token(t_lexer *current, t_vars **vars)
{
	char	*str;

	str = current->str;
	// Eğer token bir tek tırnak (') ile başlıyorsa, işleme devam etme
	if (str[0] == '\'')
		return ;
	// eğer token içinde = varsa ve kendisinden önce bir token varsa
	if (ft_strchr(str, '=') && current->prev)
	{
		t_lexer *start = current;
		// Önceki token'ı kontrol et
		// Eğer önceki token bir pipe olana kadar veya bitine kadar, başa git
		while (start->prev && start->prev->token != PIPE)
			start = start->prev;
		// Eğer kendsinden önceki token bir export değilse
		// ve kendisinden önce bir token varsa
		// içindeki dolar işareti varsa işle ve bitir
		if (!start->str || ft_strncmp(start->str, "export", 7) != 0)
		{
			current->str = is_dolar(str, vars);
			return;
		}
	}
	// Eğer token içinde = varsa ve kendisinden önce bir token yoksa
	// veya kendisinden önceki token export ise
	// değişkeni bul ve işle
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
		add_command(cmd_list, new_cmd);
	}
}
