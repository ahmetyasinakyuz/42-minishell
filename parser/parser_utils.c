/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:32 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/21 18:06:17 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * Yeni bir komut yapısı oluşturur ve başlatır.
 * Bu fonksiyon, parser tarafından kullanılmak üzere yeni bir
 * komut yapısı için bellek ayırır ve tüm alanlarını varsayılan değerlerle başlatır.
 * 
 * @param cmd Başlatılacak komut yapısının referansı
 * @return 0: Başarılı, 1: Bellek hatası
 */
int	init_cmd(t_simple_cmds **cmd)
{
	*cmd = malloc(sizeof(t_simple_cmds));
	if (!*cmd)
		return (1);
	(*cmd)->num_redirections = 0;
	(*cmd)->hd_file_name = NULL;
	(*cmd)->redirections = NULL;
	(*cmd)->next = NULL;
	(*cmd)->prev = NULL;
	(*cmd)->pipe = 0;
	(*cmd)->str = NULL;
	(*cmd)->flag = NULL;
	(*cmd)->input_type = IO_STDIN;
	(*cmd)->output_type = IO_STDOUT;
	(*cmd)->input_fd = 0;
	(*cmd)->output_fd = 1;
	(*cmd)->return_value = 0;
	return (0);
}

/**
 * Belirli bir token aralığındaki kelimelerin sayısını hesaplar.
 * Bu fonksiyon, verilen başlangıç ve bitiş token'ları arasında
 * komut yapısına eklenecek kelime sayısını hesaplar.
 * Yönlendirme argümanlarını ve bayrak olan kelimeleri saymaz.
 * 
 * @param start Sayılmaya başlanacak ilk token
 * @param end Sayımın biteceği token (dahil değil)
 * @return Bulunan kelime sayısı
 */
int	count_words(t_lexer *start, t_lexer *end)
{
	int		word_count;
	t_lexer	*current;

	word_count = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && (!current->prev
				|| !is_redirection(current->prev->token))
			&& !is_flag(current->str))
			word_count++;
		current = current->next;
	}
	return (word_count);
}

/**
 * Komut yapısının kelime dizisini doldurur.
 * Bu fonksiyon, belirtilen token aralığından alınan kelimeleri
 * komut yapısının str dizisine kopyalar. Kopyalama sırasında
 * kelimelerdeki tırnak işaretleri kaldırılır.
 * 
 * @param cmd Doldurulacak komut yapısı
 * @param start Kelimelerin alınacağı ilk token
 * @param end Kelimelerin alınacağı son token (dahil değil)
 */
void	fill_words(t_simple_cmds *cmd, t_lexer *start, t_lexer *end)
{
	int		i;
	t_lexer	*current;
	char	*unquoted;
	char	*temp;

	i = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && (!current->prev
				|| !is_redirection(current->prev->token)))
		{
			temp = ft_strdup(current->str);
			if (!is_flag(temp))
			{
				unquoted = remove_quotes(temp);
				cmd->str[i++] = unquoted;
			}
			else
				free(temp);
		}
		current = current->next;
	}
	cmd->str[i] = NULL;
}

/**
 * Bir metinden tırnak işaretlerini kaldırır.
 * Bu fonksiyon, verilen metindeki tek ve çift tırnak işaretlerini
 * kaldırarak yeni bir metin oluşturur.
 * 
 * @param str Tırnak işaretleri kaldırılacak metin
 * @return Tırnak işaretleri kaldırılmış yeni metin
 */
char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*result;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	process_quotes(str, result, &i, &j);
	free(str);
	return (result);
}
