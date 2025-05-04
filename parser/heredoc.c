/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:30:18 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/05 02:37:14 by aakyuz           ###   ########.fr       */
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

// Child process function to handle heredoc input
static void	child_heredoc_process(int fd, char *delimiter, t_vars *vars)
{
	char	*line;
	struct sigaction sa;

	// Setup proper signal handling for heredoc input
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	
	while (1)
	{
		line = readline("> ");
		if (!line)  // EOF or error
			exit(EXIT_SUCCESS);
		
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break;
		}
		
		line = is_dolar(line, &vars);
		ft_putendl_fd(line, fd);
		free(line);
	}
	close(fd);
	exit(EXIT_SUCCESS);
}

char	*create_heredoc_file(char *delimiter, t_vars *vars)
{
	pid_t	pid;
	char	*filename;
	int		fd;
	int		status;
	struct sigaction	old_int, old_quit;

	filename = generate_temp_filename();
	if (!filename)
		return (NULL);
	
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(filename);
		return (NULL);
	}

	// Save current signal handlers
	sigaction(SIGINT, NULL, &old_int);
	sigaction(SIGQUIT, NULL, &old_quit);
	
	// Set special signal handlers for heredoc
	struct sigaction sa_int;
	sa_int.sa_handler = SIG_IGN;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	pid = fork();
	if (pid == -1)
	{
		// Restore signal handlers
		sigaction(SIGINT, &old_int, NULL);
		sigaction(SIGQUIT, &old_quit, NULL);
		close(fd);
		free(filename);
		return (NULL);
	}
	
	if (pid == 0)  // Child process
		child_heredoc_process(fd, delimiter, vars);
	else  // Parent process
	{
		close(fd);  // Parent doesn't need the file descriptor
		waitpid(pid, &status, 0);
		
		// Restore signal handlers
		sigaction(SIGINT, &old_int, NULL);
		sigaction(SIGQUIT, &old_quit, NULL);
		
		// If child was terminated by a signal (Ctrl+C)
		if (WIFSIGNALED(status))
		{
			unlink(filename);  // Remove temp file
			free(filename);
			g_received_signal = SIGINT;
			return (NULL);
		}
	}
	
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
