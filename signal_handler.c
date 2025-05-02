/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 10:40:00 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/02 16:57:31 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//Derleyiciye, g_received_signal değişkeninin değerinin beklenmedik zamanlarda değişebileceğini bildirir.
volatile sig_atomic_t	g_received_signal = 0;

void	handle_sigint(int signum)
{
	//ctrl + c sinyali geldiğinde g_received_signal değişkenine sinyalin numarası atanır
	g_received_signal = signum;
	//ctrl + c sinyali geldiğinde terminalde yeni bir satır açılır
	write(STDOUT_FILENO, "\n", 1);
	
	//Yeni bir satır başlatıldığını readline (rl) kütüphanesine bildiriyor.
	rl_on_new_line();
	//readline girdisini temizler
	rl_replace_line("", 0);
	//readline girdisini yeniden ekrana yazdırıyor, yani terminalde temizlenmiş bir satır görüntülenmesini sağlıyor.
	rl_redisplay();
}

void	setup_signals(void)
{
	//ctrl + c ve ctrl + \ sinyalleri için sigaction yapısı oluşturuldu
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	//SIGINT sinyali için handle_sigint fonksiyonunu çağır
	sa_int.sa_handler = handle_sigint;

	//sa_int tarafından kullanılan sinyal maskesi temizleniyor.
	//Böylece başka sinyallerin de engellenmeden çalışmasına izin veriliyor.
	sigemptyset(&sa_int.sa_mask);

	//özel flaler olmadığı için 0
	sa_int.sa_flags = 0;

	//SIGINT sinyali için sa_int yapılandırması işletim sistemine bildiriliyor.
	sigaction(SIGINT, &sa_int, NULL);

	//SIGQUIT sinyali tamamen yok sayılıyor (SIG_IGN ile)
	sa_quit.sa_handler = SIG_IGN;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	setup_child_signals(void)
{
	struct sigaction	sa;

	// Reset SIGINT to default behavior
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);

	// Reset SIGQUIT to default behavior so it can produce core dump in child processes
	sigaction(SIGQUIT, &sa, NULL);
}

void	reset_signal_handling(void)
{
	g_received_signal = 0;
}
