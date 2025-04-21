/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 06:50:01 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/21 18:06:12 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * Yeni bir token (sözcük birimi) oluşturur.
 * Bu fonksiyon, lexer analizi sırasında belirlenen bir metni
 * token yapısına dönüştürür. Token, komut satırı girişinin anlamlı bir parçasıdır.
 * 
 * @param str Token'ın içeriği olacak metin
 * @param token_type Token'ın türü (komut, argüman, pipe, yönlendirme, vs.)
 * @param index Token'ın orijinal komut satırındaki indeksi/konumu
 * @return Oluşturulan yeni token. Bellek yetersizliği durumunda NULL döner
 */
t_lexer	*create_token(char *str, t_tokens token_type, int index)
{
	t_lexer	*new_token;

	new_token = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(str);
	new_token->token = token_type;
	new_token->i = index;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

/**
 * Token'ı mevcut listeye ekler.
 * Bu fonksiyon, oluşturulan bir token'ı çift yönlü bağlı listeye ekler.
 * Eğer liste boşsa, token ilk eleman olarak ayarlanır.
 * Liste doluysa, token listenin sonuna eklenir.
 * 
 * @param list Token'ın ekleneceği listenin referansı
 * @param new_token Listeye eklenecek yeni token
 */
void	add_token(t_lexer **list, t_lexer *new_token)
{
	t_lexer	*temp;

	if (!*list)
	{
		*list = new_token;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
	new_token->prev = temp;
}
