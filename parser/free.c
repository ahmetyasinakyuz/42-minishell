/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 06:35:43 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/21 18:32:59 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Lexer listesini serbest bırakır.
 * Bu fonksiyon, lexer analiz aşamasında oluşturulan bağlı listeyi temizler.
 * Her düğümün içindeki string değeri ve ardından düğümün kendisini serbest bırakır.
 * 
 * @param list Serbest bırakılacak lexer listesinin başlangıç pointer'ı
 */
void	free_lexer_list(t_lexer *list)
{
	t_lexer	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		free(temp->str);
		free(temp);
	}
}

/**
 * Komut listesini serbest bırakır.
 * Bu fonksiyon, parser aşamasında oluşturulan komut listesini temizler.
 * Her komut yapısının içindeki tüm dinamik olarak ayrılmış bellek alanlarını serbest bırakır:
 * - Komut dizileri (str)
 * - Flag dizileri
 * - Heredoc dosya isimleri
 * - Yönlendirmeler için ayrılmış lexer yapıları
 * 
 * @param list Serbest bırakılacak komut listesinin başlangıç pointer'ı
 */
void	free_command_list(t_simple_cmds *list)
{
	t_simple_cmds	*temp;
	int				i;

	while (list)
	{
		temp = list;
		list = list->next;
		i = 0;
		while (temp->str && temp->str[i])
			free(temp->str[i++]);
		if (temp->str)
			free(temp->str);
		i = 0;
		while (temp->flag && temp->flag[i])
			free(temp->flag[i++]);
		if (temp->flag)
			free(temp->flag);
		if (temp->hd_file_name)
			free(temp->hd_file_name);
		if (temp->redirections)
			free_lexer_list(temp->redirections);
		free(temp);
	}
}

/**
 * Değişken listesini temizler.
 * Bu fonksiyon, kabukta tanımlanan değişkenlerin bulunduğu listeyi serbest bırakır.
 * Her değişken düğümünün anahtar (key), değer (value) alanlarını ve 
 * ardından düğümün kendisini serbest bırakır.
 * 
 * @param vars Temizlenecek değişken listesinin pointer'ının referansı
 */
void	clear_vars(t_vars **vars)
{
	t_vars	*temp;

	while (*vars)
	{
		temp = *vars;
		*vars = (*vars)->next;
		free(temp->key);
		free(temp->value);
		free(temp);
	}
}
