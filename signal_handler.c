/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 10:40:00 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/05 08:39:13 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Derleyiciye, g_received_signal değişkeninin değerinin beklenmedik zamanlarda değişebileceğini bildirir.
volatile sig_atomic_t	g_received_signal = 0;

void	handle_sigint(int signum)
{
	g_received_signal = signum;
	
	// Ensure visible newline
	write(STDOUT_FILENO, "\n", 1);
	
	// Reset readline state
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay(); 
}

// Ana süreç (interaktif kabuk) için sinyal ayarları
void	setup_signals(void)
{
	//ctrl + c ve ctrl + \ sinyalleri için sigaction yapısı oluşturuldu
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	//SIGINT sinyali için handle_sigint fonksiyonunu çağır
	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	//SIGQUIT sinyali tamamen yok sayılıyor (SIG_IGN ile)
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

// Alt süreçler (komut çalıştırma) için sinyal ayarları
void	setup_child_signals(void)
{
	struct sigaction	sa;

	// SIGINT ve SIGQUIT için varsayılan davranışa geri dön
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

// Ensure we ignore SIGINT during execution
void	setup_execute_signals(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	// SIGINT is ignored during execute - parent will handle child termination
	sa_int.sa_handler = SIG_IGN; 
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = 0;
	sigaction(SIGINT, &sa_int, NULL);

	// SIGQUIT is also ignored
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	reset_signal_handling(void)
{
	g_received_signal = 0;
	// Important - clear any readline state that might be pending
}
