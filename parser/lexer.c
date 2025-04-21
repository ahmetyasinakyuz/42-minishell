/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:24 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/21 15:21:40 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Bir metin parçasının token tipini belirler.
 * Bu fonksiyon, verilen metni analiz ederek hangi tip token olduğunu tespit eder
 * (pipe, yönlendirme, sözcük vb.).
 * 
 * @param str Tipi belirlenecek metin parçası
 * @return Tespit edilen token tipi (t_tokens enum değeri)
 */
t_tokens	check_token_type(char *str)
{
	if (!str)
		return (WORD);
	if (ft_strncmp(str, "|", 2) == 0)
		return (PIPE);
	if (ft_strncmp(str, "<<", 3) == 0)
		return (REDIRECT_HEREDOC);
	if (ft_strncmp(str, "<", 2) == 0)
		return (REDIRECT_IN);
	if (ft_strncmp(str, ">>", 3) == 0)
		return (REDIRECT_APPEND);
	if (ft_strncmp(str, ">", 2) == 0)
		return (REDIRECT_OUT);
	return (WORD);
}

/**
 * Girdi metnini token'lara ayırır.
 * Bu fonksiyon, kullanıcı tarafından girilen metni parçalara ayırarak
 * her bir parçayı uygun token tipine dönüştürür ve bir token listesi oluşturur.
 * 
 * @param input Ayrıştırılacak girdi metni
 * @return Oluşturulan token listesi
 */
t_lexer	*lexer_tokenize(char *input)
{
	t_lexer		*token_list;
	char		*token;
	int			i;
	t_tokens	token_type;

	token_list = NULL;
	i = 0;
	while (input[i])
	{
		token = extract_token(input, &i);
		if (token && *token)
		{
			token_type = check_token_type(token);
			add_token(&token_list, create_token(token, token_type, i));
			free(token);
		}
		else if (token)
			free(token);
	}
	return (token_list);
}

/**
 * Lexer analizinden önce girdi metnini doğrular.
 * Bu fonksiyon, kullanıcı tarafından girilen metni lexer analizine sokmadan önce
 * sözdizimi hatalarını kontrol eder (kapatılmamış tırnak işaretleri,
 * izin verilmeyen özel karakterler vb.).
 * 
 * @param input Doğrulanacak girdi metni
 * @return Hata durumunda NULL, sorun yoksa token listesini işaret eden geçici pointer
 */
t_lexer	*lexer_precheck(char *input)
{
	if (validate_quotes(input) == QUOTE_ERROR)
	{
		printf("Error: Unclosed quotes\n");
		return (NULL);
	}
	return ((t_lexer *)1);
}

/**
 * Girdi metnini lexer analizi sürecinden geçirir.
 * Bu fonksiyon, kullanıcı tarafından girilen metni önce doğrular,
 * ardından token'lara ayırır ve bir token listesi oluşturur.
 * 
 * @param input Analiz edilecek girdi metni
 * @return Oluşturulan token listesi, hata durumunda NULL
 */
t_lexer	*lexer(char *input)
{
	if (!lexer_precheck(input))
		return (NULL);
	return (lexer_tokenize(input));
}
