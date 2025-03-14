/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:14:44 by aakyuz            #+#    #+#             */
/*   Updated: 2025/03/10 07:59:46 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

/**
 * Sözcük token'larını işler.
 * Bu fonksiyon, lexer analizi sonucu oluşturulan bir sözcük token'ını işler.
 * Eğer tek tırnak içinde değilse, içindeki değişken referanslarını bulur ve değerleri ile değiştirir.
 * 
 * @param current İşlenecek token
 * @param vars Değişken listesi
 */
void	handle_word_token(t_lexer *current, t_vars **vars)
{
	char	*str;

	str = current->str;
	if (str[0] == '\'')
		return ;
	found_var(str, vars);
	current->str = is_dolar(str, vars);
}

/**
 * Pipe token'larını işler.
 * Bu fonksiyon, bir pipe (|) token'ı ile karşılaştığında:
 * 1. Önceki token'lardan bir komut yapısı oluşturur
 * 2. Komutu pipe özelliği ile işaretler
 * 3. Output yönlendirmesini pipe olarak ayarlar
 * 4. Oluşturulan komutu komut listesine ekler
 * 
 * @param current İşlenen token'ın pointer'ının referansı (pipe token'ı)
 * @param start İşlenen komutun başlangıç token'ının referansı
 * @param cmd_list Komut listesinin referansı
 */
void	handle_pipe_token(t_lexer **current, t_lexer **start,
		t_simple_cmds **cmd_list)
{
	t_simple_cmds	*new_cmd;

	new_cmd = create_command(*start, *current);
	if (new_cmd)
	{
		new_cmd->pipe = 1;
		new_cmd->output_type = IO_PIPE_OUT;
		new_cmd->output_fd = -1;
		add_command(cmd_list, new_cmd);
	}
	*start = (*current)->next;
}

/**
 * Son token'ı işler.
 * Bu fonksiyon, lexer analizinin sonundaki token'ları işler:
 * 1. Kalan token'lardan son bir komut yapısı oluşturur
 * 2. Eğer önceki bir komut pipe ile bağlıysa, input yönlendirmesini pipe olarak ayarlar
 * 3. Oluşturulan komutu komut listesine ekler
 * 
 * @param start İşlenen son komutun başlangıç token'ının referansı
 * @param end İşlenen son komutun bitiş token'ı
 * @param cmd_list Komut listesinin referansı
 */
void	handle_last_token(t_lexer **start, t_lexer *end,
		t_simple_cmds **cmd_list)
{
	t_simple_cmds	*new_cmd;

	new_cmd = create_command(*start, end);
	if (new_cmd)
	{
		new_cmd->pipe = 0;
		if (*cmd_list && (*cmd_list)->pipe)
		{
			new_cmd->input_type = IO_PIPE_IN;
			new_cmd->input_fd = -1;
		}
		add_command(cmd_list, new_cmd);
	}
}

/**
 * Alfanümerik olmayan dolar işaretlerini işler.
 * Bu fonksiyon, $ işaretinden sonra alfanümerik olmayan karakterler olduğunda,
 * bu karakterleri atlar ve dolar işareti sonrasındaki alfanümerik kısımla devam eder.
 * 
 * @param result İşlenecek olan metin
 * @param i Metin içindeki konum referansı
 * @return İşlenmiş metin
 */
char	*handle_non_alpha_dollar(char *result, int *i)
{
	int		j;
	char	*temp;

	j = *i;
	while (result[j] && !ft_isalnum(result[j]))
		j++;
	temp = ft_strdup(result);
	free(result);
	result = ft_strjoin(ft_substr(temp, 0, *i), temp + j);
	free(temp);
	*i = -1;
	return (result);
}
