/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils_extra.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmtemel <ahmtemel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 13:03:02 by ahmtemel          #+#    #+#             */
/*   Updated: 2025/05/11 13:15:08 by ahmtemel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	free_buffer(char **buffer)
{
	int	i;

	i = 0;
	while (buffer && buffer[i])
		free(buffer[i++]);
	free(buffer);
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
