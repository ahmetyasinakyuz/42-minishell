/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:30:18 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/21 15:21:35 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>
#include <readline/readline.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Geçici bir heredoc dosyası oluşturur.
 * 
 * @return Oluşturulan geçici dosyanın adı veya NULL (hata durumunda)
 */
static char	*generate_temp_filename(void)
{
	static int	heredoc_count = 0;
	char		*count_str;
	char		*filename;

	count_str = ft_itoa(heredoc_count++);
	if (!count_str)
		return (NULL);
	filename = ft_strjoin("/tmp/heredoc_", count_str);
	free(count_str);
	return (filename);
}

/**
 * Heredoc için girişleri işler ve bir dosyaya yazar.
 * Belirtilen sınırlayıcı (delimiter) ile tam olarak eşleşen satır görülene
 * kadar kullanıcıdan giriş okur.
 * 
 * @param fd Yazılacak dosyanın tanımlayıcısı
 * @param delimiter Heredoc sınırlayıcısı
 */
void	process_heredoc_input(int fd, char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strncmp(line, delimiter, ft_strlen(delimiter) + 1) == 0)
		{
			free(line);
			break ;
		}
		ft_putendl_fd(line, fd);
		free(line);
	}
}

/**
 * Belirtilen sınırlayıcı ile bir heredoc dosyası oluşturur.
 * Kullanıcıdan giriş alır ve geçici bir dosyaya yazar.
 * 
 * @param delimiter Heredoc sınırlayıcısı
 * @return Oluşturulan heredoc dosyasının adı
 */
char	*create_heredoc_file(char *delimiter)
{
	char	*filename;
	int		fd;

	filename = generate_temp_filename();
	if (!filename)
		return (NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		free(filename);
		return (NULL);
	}
	process_heredoc_input(fd, delimiter);
	close(fd);
	return (filename);
}

/**
 * Komutun heredoc yönlendirmelerini işler.
 * 
 * @param cmd Heredoc'un işleneceği komut
 * @param redirections Yönlendirme listesi
 */
void	handle_heredoc(t_simple_cmds *cmd, t_lexer *redirections)
{
	t_lexer	*current;
	char	*delimiter;
	char	*filename;

	current = redirections;
	while (current)
	{
		if (current->token == REDIRECT_HEREDOC && current->next)
		{
			delimiter = current->next->str;
			// Tırnak işaretlerini kaldır
			delimiter = remove_quotes(ft_strdup(delimiter));
			filename = create_heredoc_file(delimiter);
			free(delimiter);
			
			if (filename)
			{
				// Önceki heredoc dosyasını temizle
				if (cmd->hd_file_name)
					free(cmd->hd_file_name);
				cmd->hd_file_name = filename;
				cmd->input_type = IO_HEREDOC;
				cmd->input_fd = -1;
			}
		}
		if (current->next)
			current = current->next->next;
		else
			break;
	}
}
