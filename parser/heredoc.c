/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:30:18 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/05 08:21:38 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <setjmp.h>

// For signaling between handler and main code
static jmp_buf heredoc_jmp;

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

// Updated signal handler
static void	heredoc_sigint_handler(int signum)
{
	(void)signum;
	g_received_signal = SIGINT;
	// Jump back to where setjmp was called
	siglongjmp(heredoc_jmp, 1);
}

// Define an event hook for readline to check for our signal
static int heredoc_event_hook(void)
{
	if (g_received_signal == SIGINT)
		rl_done = 1;
	return 0;
}

char	*create_heredoc_file(char *delimiter, t_vars *vars)
{
	char	*filename;
	int		fd;
	struct sigaction	old_int, old_quit;
	struct sigaction	sa_int, sa_quit;
	char	*line;

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
	
	// Setup custom signal handlers
	sa_int.sa_handler = heredoc_sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
	
	// Set up event hook for readline to check for signals
	rl_event_hook = heredoc_event_hook;
	
	// Reset signal flag
	g_received_signal = 0;
	
	// Use setjmp to allow jumping back here on SIGINT
	if (sigsetjmp(heredoc_jmp, 1) == 0)
	{
		while (1)
		{
			line = readline("> ");
			
			if (!line || g_received_signal == SIGINT)
				break;
			
			if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
			{
				free(line);
				break;
			}
			
			line = is_dolar(line, &vars);
			ft_putendl_fd(line, fd);
			free(line);
		}
	}
	
	close(fd);
	
	// Restore signal handlers
	sigaction(SIGINT, &old_int, NULL);
	sigaction(SIGQUIT, &old_quit, NULL);
	
	// Reset the readline event hook
	rl_event_hook = NULL;
	
	// If we received a signal, clean up and return NULL
	if (g_received_signal == SIGINT)
	{
		unlink(filename);
		free(filename);
		write(1, "\n", 1);  // For clean output
		return (NULL);
	}
	
	return (filename);
}

void	process_single_heredoc(t_simple_cmds *cmd, t_lexer *current, t_vars *vars)
{
	char	*delimiter;
	char	*filename;

	if (current->token == REDIRECT_HEREDOC && current->next)
	{
		delimiter = ft_strdup(current->next->str);
		if (!delimiter)
			return;
		
		delimiter = remove_quotes(delimiter);
		
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
		
		if (g_received_signal == SIGINT)
			break;
		
		if (current->next)
			current = current->next->next;
		else
			break;
	}
}
