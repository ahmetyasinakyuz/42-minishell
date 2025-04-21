/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flags.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:53:22 by akyuz             #+#    #+#             */
/*   Updated: 2025/04/21 18:32:57 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Verilen metnin bir bayrak (flag) olup olmadığını kontrol eder.
 * Bayrak formatı: '-' ile başlayıp ardından en az bir karakter gelmelidir.
 * '--' ile başlayan metinler bayrak olarak kabul edilmez.
 * 
 * @param str Kontrol edilecek metin.
 * @return Metin bir bayrak ise 1, değilse 0 döner.
 */
int	is_flag(char *str)
{
	char	*unquoted;
	int		result;

	if (!str)
		return (0);
	unquoted = remove_quotes(ft_strdup(str));
	result = (unquoted && unquoted[0] == '-' && unquoted[1]
			&& unquoted[1] != '-');
	free(unquoted);
	return (result);
}

/**
 * Verilen lexer aralığındaki bayrak sayısını hesaplar.
 * 
 * @param start İşlenecek lexer listesinin başlangıç noktası.
 * @param end İşlenecek lexer listesinin bitiş noktası.
 * @return Bulunan bayrak sayısı.
 */
int	count_flags(t_lexer *start, t_lexer *end)
{
	int		flag_count;
	t_lexer	*current;

	flag_count = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && is_flag(current->str) && (!current->prev
				|| current->prev->token == WORD))
			flag_count++;
		current = current->next;
	}
	return (flag_count);
}

/**
 * Komut yapısının bayrak dizisini, lexer listesindeki bayraklarla doldurur.
 * 
 * @param cmd Bayrakları doldurulacak komut yapısı.
 * @param start İşlenecek lexer listesinin başlangıç noktası.
 * @param end İşlenecek lexer listesinin bitiş noktası.
 */
void	fill_flags(t_simple_cmds *cmd, t_lexer *start, t_lexer *end)
{
	int		i;
	t_lexer	*current;
	char	*unquoted;

	i = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && is_flag(current->str) && (!current->prev
				|| current->prev->token == WORD))
		{
			unquoted = remove_quotes(ft_strdup(current->str));
			cmd->flag[i++] = unquoted;
		}
		current = current->next;
	}
	cmd->flag[i] = NULL;
}
