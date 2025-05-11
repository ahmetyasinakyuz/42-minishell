/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 12:13:38 by aycami            #+#    #+#             */
/*   Updated: 2025/05/11 12:23:04 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	restore_heredoc_signals(int original_signal)
{
	setup_signals();
	g_received_signal = original_signal;
}

void	handle_heredoc_sigint(int signum)
{
	g_received_signal = signum;
	write(STDOUT_FILENO, "\n", 1);
	close(0);
}

void	handle_sigint(int signum)
{
	g_received_signal = signum;
	write(STDOUT_FILENO, "\n", 1);
	if (isatty(STDIN_FILENO))
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_continuation_sigint(int signum)
{
	g_received_signal = signum;
	write(STDOUT_FILENO, "\n", 1);
	close(0);
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}
