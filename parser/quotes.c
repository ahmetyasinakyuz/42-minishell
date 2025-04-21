/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 12:01:57 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/21 15:21:42 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Tırnaklı bir metni işler.
 * Bu fonksiyon, bir metinde tırnak işaretiyle başlayan bir bölümü işler.
 * Tırnağın kapanışını arar ve eğer kapanış tırnak işareti bulunamazsa hata döndürür.
 * 
 * @param str İşlenecek metin
 * @param i Metin içindeki konum referansı
 * @param quote İşlenecek tırnak karakteri (' veya ")
 * @return SUCCESS: İşlem başarılı, QUOTE_ERROR: Kapanmamış tırnak hatası
 */
int	handle_quoted_string(char *str, int *i, char quote)
{
	(*i)++;
	while (str[*i] && str[*i] != quote)
		(*i)++;
	if (!str[*i])
		return (QUOTE_ERROR);
	return (SUCCESS);
}

/**
 * Bir metindeki tırnak işaretlerinin doğru kapatılıp kapatılmadığını kontrol eder.
 * Bu fonksiyon, metindeki tek ve çift tırnak işaretlerinin doğru şekilde
 * açılıp kapandığını doğrular.
 * 
 * @param str Doğrulanacak metin
 * @return SUCCESS: Tırnak işaretleri doğru, QUOTE_ERROR: Hatalı tırnak işareti kullanımı
 */
int	validate_quotes(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			if (handle_quoted_string(str, &i, '\'') == QUOTE_ERROR)
				return (QUOTE_ERROR);
		}
		else if (str[i] == '\"')
		{
			if (handle_quoted_string(str, &i, '\"') == QUOTE_ERROR)
				return (QUOTE_ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

/**
 * Tırnak işareti durumunu günceller.
 * Bu fonksiyon, metin işlenirken tırnak işareti durumunu (tek tırnak içinde mi,
 * çift tırnak içinde mi) takip eder ve gerektiğinde günceller.
 * 
 * @param input İşlenen metin
 * @param i Metin içindeki konum referansı
 * @param in_squote Tek tırnak içinde olma durumu referansı
 * @param in_dquote Çift tırnak içinde olma durumu referansı
 */
void	handle_quote_status(char *input, int *i, int *in_squote, int *in_dquote)
{
	if (input[*i] == '\'' && !(*in_dquote))
		*in_squote = !(*in_squote);
	else if (input[*i] == '\"' && !(*in_squote))
		*in_dquote = !(*in_dquote);
}

/**
 * Bir metinden tırnak işaretlerini kaldırır ve sonucu yeni bir diziye kopyalar.
 * Bu fonksiyon, metni tarar ve tırnak işaretlerini kaldırarak içeriği
 * yeni bir sonuç dizisine kopyalar.
 * 
 * @param str İşlenecek metin
 * @param result Tırnak işaretleri kaldırılmış içeriğin yazılacağı dizi
 * @param i Kaynak metin içindeki konum referansı
 * @param j Sonuç dizisi içindeki konum referansı
 */
void	process_quotes(char *str, char *result, int *i, int *j)
{
	int	in_squote;
	int	in_dquote;

	in_squote = 0;
	in_dquote = 0;
	while (str[*i])
	{
		if (str[*i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (str[*i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		else
			result[(*j)++] = str[*i];
		(*i)++;
	}
	result[*j] = '\0';
}
