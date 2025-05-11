/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:30:18 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/11 12:20:19 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*generate_temp_filename(void)
{
	static int	heredoc_count = 0;
	char		*count_str;
	char		*filename;

	count_str = ft_itoa(heredoc_count++);
	if (!count_str)
		return (NULL);
	filename = ft_strjoin("/tmp/heredoc_", count_str);
	free(count_str);
	return (filename);
}

int	setup_heredoc_signals_and_save(void)
{
	int	original_signal;

	original_signal = g_received_signal;
	setup_heredoc_signals();
	return (original_signal);
}

char	*create_heredoc_file(char *delimiter, t_vars *vars)
{
	char	*filename;
	int		fd;
	int		original_stdin;

	fd = open_heredoc_file(&filename);
	if (fd == -1)
		return (NULL);
	original_stdin = dup(STDIN_FILENO);
	process_heredoc_input(fd, delimiter, vars);
	if (g_received_signal == SIGINT)
	{
		cleanup_on_interrupt(fd, filename, original_stdin);
		add_static_var(&vars, "?", "130");
		return (NULL);
	}
	close(fd);
	restore_stdin(original_stdin);
	return (filename);
}

void	process_single_heredoc(t_simple_cmds *cmd, t_lexer *current,
		t_vars *vars)
{
	char	*delimiter;
	char	*filename;

	if (current->token == REDIRECT_HEREDOC && current->next)
	{
		delimiter = current->next->str;
		delimiter = remove_quotes(ft_strdup(delimiter));
		filename = create_heredoc_file(delimiter, vars);
		free(delimiter);
		if (filename)
		{
			if (cmd->hd_file_name)
				free(cmd->hd_file_name);
			cmd->hd_file_name = filename;
			cmd->input_type = IO_HEREDOC;
			cmd->input_fd = -1;
		}
	}
}

void	handle_heredoc(t_simple_cmds *cmd, t_lexer *redirections, t_vars *vars)
{
	t_lexer	*current;

	current = redirections;
	while (current)
	{
		process_single_heredoc(cmd, current, vars);
		if (current->next)
			current = current->next->next;
		else
			break ;
	}
}
