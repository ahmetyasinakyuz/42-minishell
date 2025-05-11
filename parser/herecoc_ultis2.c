/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herecoc_ultis2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 12:51:59 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/11 12:52:34 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	**init_heredoc_buffer(void)
{
	char	**buffer;

	buffer = malloc(sizeof(char *));
	if (!buffer)
		return (NULL);
	buffer[0] = NULL;
	return (buffer);
}

void	process_heredoc_input(int fd, char *delimiter, t_vars *vars)
{
	char	*line;
	int		original_signal;
	int		continue_reading;
	char	**buffer;
	int		sigint_received;

	buffer = init_heredoc_buffer();
	if (!buffer)
		return ;
	sigint_received = 0;
	original_signal = setup_heredoc_signals_and_save();
	continue_reading = 1;
	while (continue_reading)
	{
		line = readline("> ");
		continue_reading = handle_heredoc_line(line, delimiter, vars, &buffer);
		if (continue_reading == -1)
		{
			sigint_received = 1;
			continue_reading = 0;
		}
	}
	write_buffer_to_fd(fd, buffer, sigint_received);
	free(buffer);
	restore_heredoc_signals(original_signal);
}

int	open_heredoc_file(char **filename)
{
	int	fd;

	*filename = generate_temp_filename();
	if (!*filename)
		return (-1);
	fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(*filename);
		return (-1);
	}
	return (fd);
}

void	cleanup_on_interrupt(int fd, char *filename, int original_stdin)
{
	close(fd);
	unlink(filename);
	free(filename);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
}

void	restore_stdin(int original_stdin)
{
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
}
