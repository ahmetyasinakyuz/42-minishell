/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:30:18 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/04 12:51:22 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*generate_temp_filename(void)
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

void	process_heredoc_input(int fd, char *delimiter, t_vars *vars)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		line = is_dolar(line, &vars);
		ft_putendl_fd(line, fd);
		free(line);
	}
}

char	*create_heredoc_file(char *delimiter, t_vars *vars)
{
	char	*filename;
	int		fd;

	filename = generate_temp_filename();
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(filename);
		return (NULL);
	}
	process_heredoc_input(fd, delimiter, vars);
	close(fd);
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
