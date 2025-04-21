/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:45:18 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/21 18:33:33 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Komutun G/Ç (I/O) tipini günceller.
 * Bu fonksiyon, belirtilen token tipine göre komutun input veya output
 * tipini ve dosya tanımlayıcısını günceller.
 * 
 * @param cmd Güncellenecek komut
 * @param token Yönlendirme token tipi
 */
static void	update_io_type(t_simple_cmds *cmd, t_tokens token)
{
	if (token == REDIRECT_IN)
	{
		cmd->input_type = IO_FILE_IN;
		cmd->input_fd = -1;
	}
	else if (token == REDIRECT_OUT)
	{
		cmd->output_type = IO_FILE_OUT;
		cmd->output_fd = -1;
	}
	else if (token == REDIRECT_APPEND)
	{
		cmd->output_type = IO_APPEND;
		cmd->output_fd = -1;
	}
	else if (token == REDIRECT_HEREDOC)
	{
		cmd->input_type = IO_HEREDOC;
		cmd->input_fd = -1;
	}
}

/**
 * Token listesindeki yönlendirmeleri işler.
 * Bu fonksiyon, token listesini tarar ve yönlendirme token'larını bulur.
 * Bulunan yönlendirmeleri komut yapısına ekler ve komutun G/Ç (I/O) tiplerini günceller.
 * 
 * @param cmd Yönlendirmelerin ekleneceği komut
 * @param token_list Taranacak token listesinin referansı
 */
void	handle_redirections(t_simple_cmds *cmd, t_lexer **token_list)
{
	t_lexer	*current;
	t_lexer	*next;

	current = *token_list;
	while (current && current->token != PIPE)
	{
		next = current->next;
		if (is_redirection(current->token))
		{
			cmd->num_redirections++;
			update_io_type(cmd, current->token);
			if (next && next->token == WORD)
			{
				add_redirection(&cmd->redirections, copy_token(current));
				add_redirection(&cmd->redirections, copy_token(next));
			}
		}
		current = next;
	}
}

/**
 * Bir token'ın yönlendirme token'ı olup olmadığını kontrol eder.
 * Bu fonksiyon, verilen token tipinin giriş (<), çıkış (>),
 * ekleme (>>) veya heredoc (<<) yönlendirme token'ı olup olmadığını belirler.
 * 
 * @param token Kontrol edilecek token tipi
 * @return 1: Token bir yönlendirme token'ı, 0: Değil
 */
int	is_redirection(t_tokens token)
{
	return (token == REDIRECT_IN || token == REDIRECT_OUT
		|| token == REDIRECT_APPEND || token == REDIRECT_HEREDOC);
}

/**
 * Yönlendirme listesine yeni bir yönlendirme token'ı ekler.
 * Bu fonksiyon, komut yapısının yönlendirme listesine yeni bir token ekler.
 * Liste boşsa, token liste başlangıcı olarak ayarlanır.
 * 
 * @param redirection_list Yönlendirme listesinin referansı
 * @param token Eklenecek yönlendirme token'ı
 */
void	add_redirection(t_lexer **redirection_list, t_lexer *token)
{
	t_lexer	*temp;

	if (!*redirection_list)
	{
		*redirection_list = token;
		return ;
	}
	temp = *redirection_list;
	while (temp->next)
		temp = temp->next;
	temp->next = token;
	token->prev = temp;
}
