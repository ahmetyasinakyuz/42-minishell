/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:35:39 by akyuz             #+#    #+#             */
/*   Updated: 2025/04/21 18:06:42 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * Bir token'ı listeden çıkarır.
 * Bu fonksiyon, belirtilen token'ı lexer listesinden çıkarır ve bağlantılarını düzenler.
 * Token'ı listeden tamamen izole eder (önceki ve sonraki bağlantılar NULL olarak ayarlanır).
 * 
 * @param list Token'ın çıkarılacağı liste referansı
 * @param token Listeden çıkarılacak token
 */
void	remove_token(t_lexer **list, t_lexer *token)
{
	if (token->prev)
		token->prev->next = token->next;
	else
		*list = token->next;
	if (token->next)
		token->next->prev = token->prev;
	token->next = NULL;
	token->prev = NULL;
}

/**
 * Bir token'ın kopyasını oluşturur.
 * Bu fonksiyon, belirtilen token'ın tüm alan değerleriyle beraber
 * yeni bir kopyasını oluşturur. Özellikle yönlendirme işlemleri için kullanılır.
 * 
 * @param token Kopyalanacak token
 * @return Oluşturulan yeni token kopyası
 */
t_lexer	*copy_token(t_lexer *token)
{
	t_lexer	*new_token;

	new_token = malloc(sizeof(t_lexer));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(token->str);
	new_token->token = token->token;
	new_token->i = token->i;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

/**
 * Pipe karakterini işler.
 * Bu fonksiyon, girdi metninde pipe karakteri (|) ile karşılaşıldığında çağrılır.
 * Pipe karakterini token'a ekler ve işlemi sonlandırır.
 * 
 * @param token Oluşturulan token
 * @param input İşlenen girdi metni
 * @param i Girdi metni içindeki konum referansı
 * @param j Token içindeki konum referansı
 * @return Token işleme durumu (1: İşlendi ve sonlandırıldı)
 */
int	handle_pipe_char(char *token, char *input, int *i, int *j)
{
	if (*j > 0)
		return (1);
	token[(*j)++] = input[(*i)++];
	return (1);
}

/**
 * Girdi metninden token karakterlerini işler.
 * Bu fonksiyon, girdi metnini tarayarak tek tırnak, çift tırnak, pipe ve boşluk karakterlerini
 * uygun şekilde işler ve token oluşturur.
 * 
 * @param token Oluşturulan token
 * @param input İşlenen girdi metni
 * @param i Girdi metni içindeki konum referansı
 * @param j Token içindeki konum referansı
 */
void	process_token_char(char *token, char *input, int *i, int *j)
{
	int	in_squote;
	int	in_dquote;

	in_squote = 0;
	in_dquote = 0;
	while (input[*i])
	{
		handle_quote_status(input, i, &in_squote, &in_dquote);
		if (input[*i] == '|' && !in_squote && !in_dquote)
			if (handle_pipe_char(token, input, i, j))
				break ;
		if ((input[*i] == ' ' && !in_squote && !in_dquote && *j > 0))
			break ;
		token[(*j)++] = input[(*i)++];
	}
}

/**
 * Girdi metninden bir token çıkarır.
 * Bu fonksiyon, girdi metninden anlam bütünlüğü olan bir parça (token) çıkarır.
 * Başlangıçtaki boşlukları atlar ve bir token oluşturana kadar metni işler.
 * 
 * @param input İşlenecek girdi metni
 * @param i Girdi metni içindeki konum referansı
 * @return Çıkarılan token metni
 */
char	*extract_token(char *input, int *i)
{
	int		j;
	char	*token;

	j = 0;
	while (input[*i] == ' ')
		(*i)++;
	token = ft_calloc(ft_strlen(input) + 1, sizeof(char));
	if (!token)
		return (NULL);
	process_token_char(token, input, i, &j);
	if (input[*i] == ' ')
		(*i)++;
	return (token);
}
