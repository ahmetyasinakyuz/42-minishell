/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_ultis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:46:20 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/11 11:07:19 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	handle_heredoc_line(char *line, char *delimiter, t_vars *vars,
		char ***buffer)
{
	char	*processed_line;
	char	**new_buffer;
	int		i;

	if (!line || g_received_signal == SIGINT)
	{
		if (g_received_signal == SIGINT)
			add_static_var(&vars, "?", "130");
		return (0);
	}
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
	{
		free(line);
		return (0);
	}
	
	processed_line = is_dolar(line, &vars);
	// Add line to buffer instead of writing directly to file
	i = 0;
	while ((*buffer) && (*buffer)[i])
		i++;
	new_buffer = malloc(sizeof(char *) * (i + 2));
	if (!new_buffer)
	{
		free(processed_line);
		return (0);
	}
	i = 0;
	while ((*buffer) && (*buffer)[i])
	{
		new_buffer[i] = (*buffer)[i];
		i++;
	}
	new_buffer[i] = processed_line;
	new_buffer[i + 1] = NULL;
	
	if (*buffer)
		free(*buffer);
	*buffer = new_buffer;
	
	return (1);
}

void	process_heredoc_input(int fd, char *delimiter, t_vars *vars)
{
	char	*line;
	int		original_signal;
	int		continue_reading;
	char	**buffer;
	int		i;

	buffer = malloc(sizeof(char *));
	if (!buffer)
		return;
	buffer[0] = NULL;
	
	original_signal = setup_heredoc_signals_and_save();
	continue_reading = 1;
	while (continue_reading)
	{
		line = readline("> ");
		continue_reading = handle_heredoc_line(line, delimiter, vars, &buffer);
	}
	
	// Only write buffer to file if no SIGINT received
	if (g_received_signal != SIGINT)
	{
		i = 0;
		while (buffer && buffer[i])
		{
			ft_putendl_fd(buffer[i], fd);
			free(buffer[i]);
			i++;
		}
	}
	else
	{
		// Free buffer contents
		i = 0;
		while (buffer && buffer[i])
			free(buffer[i++]);
	}
	
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
