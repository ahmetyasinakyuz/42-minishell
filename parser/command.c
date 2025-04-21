/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:34:53 by akyuz             #+#    #+#             */
/*   Updated: 2025/04/21 15:21:22 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

/**
 * Komut yapısı için bayrak (flag) ayarlaması yapar.
 * 
 * @param cmd Bayrakların ekleneceği komut yapısı.
 * @param start İşlenecek lexer listesinin başlangıç noktası.
 * @param end İşlenecek lexer listesinin bitiş noktası.
 * @return Bayrağı düzenlenmiş komut yapısı veya bellek hatası durumunda NULL.
 */
t_simple_cmds	*setup_command_flags(t_simple_cmds *cmd, t_lexer *start,
		t_lexer *end)
{
	int	flag_count;

	handle_redirections(cmd, &start);
	
	// Add this line to process heredoc if present
	if (cmd->redirections)
		handle_heredoc(cmd, cmd->redirections);
	
	flag_count = count_flags(start, end);
	if (flag_count > 0)
	{
		cmd->flag = malloc(sizeof(char *) * (flag_count + 1));
		if (!cmd->flag)
		{
			free(cmd);
			return (NULL);
		}
		fill_flags(cmd, start, end);
	}
	return (cmd);
}

/**
 * Verilen lexer düğümleri arasından yeni bir komut yapısı oluşturur.
 * 
 * @param start İşlenecek lexer listesinin başlangıç noktası.
 * @param end İşlenecek lexer listesinin bitiş noktası.
 * @return Oluşturulan komut yapısı veya hata durumunda NULL.
 */
t_simple_cmds	*create_command(t_lexer *start, t_lexer *end)
{
	t_simple_cmds	*cmd;
	int				word_count;

	if (init_cmd(&cmd))
		return (NULL);
	cmd = setup_command_flags(cmd, start, end);
	if (!cmd)
		return (NULL);
	word_count = count_words(start, end);
	if (word_count == 0)
	{
		free(cmd->flag);
		free(cmd);
		return (NULL);
	}
	cmd->str = malloc(sizeof(char *) * (word_count + 1));
	if (!cmd->str)
	{
		free(cmd->flag);
		free(cmd);
		return (NULL);
	}
	fill_words(cmd, start, end);
	return (cmd);
}

/**
 * Yeni bir komutu mevcut komut listesine ekler.
 * 
 * @param cmd_list Komut listesinin adresi.
 * @param new_cmd Listeye eklenecek yeni komut.
 */
void	add_command(t_simple_cmds **cmd_list, t_simple_cmds *new_cmd)
{
	t_simple_cmds	*temp;

	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		return ;
	}
	temp = *cmd_list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_cmd;
	new_cmd->prev = temp;
}
