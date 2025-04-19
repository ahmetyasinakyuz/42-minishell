/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:46 by aakyuz            #+#    #+#             */
/*   Updated: 2025/03/22 15:24:18 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libs/ft_printf/ft_printf.h"
# include "libs/get_next_line/get_next_line.h"
# include "libs/libft/libft.h"
# include "parser/parser.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>

extern volatile sig_atomic_t	g_received_signal;

void	run_shell(t_vars **vars);
void	setup_signals(void);
void	reset_signal_handling(void);
void	handle_sigint(int signum);

#endif
