/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:46 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/21 15:20:33 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libs/ft_printf/ft_printf.h"
# include "libs/get_next_line/get_next_line.h"
# include "libs/libft/libft.h"
# include "parser/parser.h"
# include "execute/execute.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>

extern volatile sig_atomic_t	g_received_signal;

void	run_shell(t_vars **vars);
void	setup_signals(void);
void	reset_signal_handling(void);
void	handle_sigint(int signum);

#endif
