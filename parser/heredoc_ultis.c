/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_ultis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 10:46:20 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/11 12:53:57 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	check_heredoc_exit_conditions(char *line)
{
	if (!line || g_received_signal == SIGINT)
	{
		if (g_received_signal == SIGINT)
			return (-1);
		return (0);
	}
	return (1);
}

static int	is_delimiter_match(char *line, char *delimiter)
{
	if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

static int	add_to_buffer(char *processed_line, char ***buffer)
{
	char	**new_buffer;
	int		i;

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

int	handle_heredoc_line(char *line, char *delimiter, t_vars *vars,
		char ***buffer)
{
	char	*processed_line;
	int		result;

	result = check_heredoc_exit_conditions(line);
	if (result <= 0)
		return (result);
	if (is_delimiter_match(line, delimiter))
		return (0);
	processed_line = is_dolar(line, &vars);
	if (!add_to_buffer(processed_line, buffer))
		return (0);
	return (1);
}

void	write_buffer_to_fd(int fd, char **buffer, int sigint_received)
{
	int	i;

	if (!sigint_received)
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
		i = 0;
		while (buffer && buffer[i])
			free(buffer[i++]);
	}
}
