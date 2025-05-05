/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_handle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 08:50:40 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 08:53:17 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	open_output_file(t_simple_cmds *cmd_list, char *filename, int flags)
{
	cmd_list->output_fd = open(filename, flags, 0644);
	if (cmd_list->output_fd < 0)
	{
		handle_file_error(cmd_list, filename, "Permission denied");
		free(filename);
		return (0);
	}
	free(filename);
	dup2(cmd_list->output_fd, STDOUT_FILENO);
	return (1);
}

static int	handle_output_redirection(t_simple_cmds *cmd_list, t_lexer *current)
{
	char	*filename;

	filename = unquote_filename(current->next->str);
	if (current->token == REDIRECT_OUT)
		return (open_output_file(cmd_list, filename,
				O_WRONLY | O_CREAT | O_TRUNC));
	else if (current->token == REDIRECT_APPEND)
		return (open_output_file(cmd_list, filename,
				O_WRONLY | O_CREAT | O_APPEND));
	free(filename);
	return (1);
}

void	o_handle(t_simple_cmds *cmd_list)
{
	t_lexer	*current;

	if (cmd_list->output_type == IO_PIPE_OUT)
		dup2(cmd_list->output_fd, STDOUT_FILENO);
	current = cmd_list->redirections;
	while (current && current->next && cmd_list->return_value == 0)
	{
		if (!handle_output_redirection(cmd_list, current))
			return ;
		current = current->next->next;
	}
}
