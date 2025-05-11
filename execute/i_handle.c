/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   i_handle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 08:50:43 by aycami            #+#    #+#             */
/*   Updated: 2025/05/11 11:05:59 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	handle_heredoc_input(t_simple_cmds *cmd_list)
{
	if (!cmd_list->hd_file_name)
		return ;
	cmd_list->input_fd = open(cmd_list->hd_file_name, O_RDONLY);
	if (cmd_list->input_fd < 0)
	{
		handle_file_error(cmd_list, "heredoc", "Cannot open temporary file");
		return ;
	}
	dup2(cmd_list->input_fd, STDIN_FILENO);
}

static int	handle_input_redirection(t_simple_cmds *cmd_list, t_lexer *current)
{
	char	*filename;

	filename = unquote_filename(current->next->str);
	if (access(filename, F_OK) != 0)
	{
		handle_file_error(cmd_list, filename, "No such file or directory");
		free(filename);
		return (0);
	}
	cmd_list->input_fd = open(filename, O_RDONLY);
	if (cmd_list->input_fd < 0)
	{
		handle_file_error(cmd_list, filename, "Permission denied");
		free(filename);
		return (0);
	}
	free(filename);
	dup2(cmd_list->input_fd, STDIN_FILENO);
	return (1);
}

void	i_handle(t_simple_cmds *cmd_list)
{
	t_lexer	*current;

	if (cmd_list->input_type == IO_PIPE_IN)
		dup2(cmd_list->input_fd, STDIN_FILENO);
	else if (cmd_list->input_type == IO_HEREDOC)
		handle_heredoc_input(cmd_list);
	current = cmd_list->redirections;
	while (current && current->next)
	{
		if (current->token == REDIRECT_IN)
		{
			if (!handle_input_redirection(cmd_list, current))
				return ;
		}
		current = current->next->next;
	}
}
