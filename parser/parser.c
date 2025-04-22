/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/22 11:40:32 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
#include <fcntl.h>

/**
 * G/Ç (I/O) tipini insan tarafından okunabilir biçimde yazdırır.
 * Bu fonksiyon, debugging amaçlı olarak kullanılır ve
 * belirtilen G/Ç tipini konsola yazdırır.
 * 
 * @param type Yazdırılacak olan G/Ç tipi enum değeri
 */
void	print_io_type(t_io_type type)
{
	if (type == IO_STDIN)
		printf("IO_STDIN");
	else if (type == IO_STDOUT)
		printf("IO_STDOUT");
	else if (type == IO_PIPE_IN)
		printf("IO_PIPE_IN");
	else if (type == IO_PIPE_OUT)
		printf("IO_PIPE_OUT");
	else if (type == IO_FILE_IN)
		printf("IO_FILE_IN");
	else if (type == IO_FILE_OUT)
		printf("IO_FILE_OUT");
	else if (type == IO_HEREDOC)
		printf("IO_HEREDOC");
	else if (type == IO_APPEND)
		printf("IO_APPEND");
}

/**
 * Heredoc dosyasının içeriğini okur ve ekrana yazdırır.
 * 
 * @param filename Okunacak heredoc dosyasının adı
 */
void	print_heredoc_content(char *filename)
{
	int		fd;
	char	*line;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf("(empty heredoc)\n");
		return ;
	}
	
	printf("heredoc content:\n");
	line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

/**
 * Komut listesinin detaylarını konsola yazdırır.
 * Bu fonksiyon, debugging amaçlı olarak kullanılır ve
 * komut listesindeki her komutun ayrıntılarını (pipe durumu, G/Ç yönlendirmeleri,
 * komut dizileri, bayraklar ve yönlendirmeler) konsola yazdırır.
 * 
 * @param cmd_list Yazdırılacak komut listesi
 */
void	print_cmd_list(t_simple_cmds *cmd_list)
{
	t_simple_cmds	*current_cmd;
	t_lexer			*current_redir;
	int				i;

	current_cmd = cmd_list;
	while (current_cmd)
	{
		printf("\n--- Command ---\n");
		printf("Pipe: %d\n", current_cmd->pipe);
		printf("Input type: ");
		print_io_type(current_cmd->input_type);
		printf(" (fd: %d)\n", current_cmd->input_fd);
		printf("Output type: ");
		print_io_type(current_cmd->output_type);
		printf(" (fd: %d)\n", current_cmd->output_fd);
		i = 0;
		while (current_cmd->str[i])
		{
			printf("cmd: %s\n", current_cmd->str[i]);
			i++;
		}
		i = 0;
		while (current_cmd->flag && current_cmd->flag[i])
		{
			printf("flag: %s\n", current_cmd->flag[i]);
			i++;
			}
		
		// Print the content array (all inputs in order)
		i = 0;
		printf("Content array (all inputs in order):\n");
		while (current_cmd->content && current_cmd->content[i])
		{
			printf("content[%d]: %s\n", i, current_cmd->content[i]);
			i++;
		}
		
		current_redir = current_cmd->redirections;
		while (current_redir && current_redir->next)
		{
			printf("redirection type: ");
			if (current_redir->token == REDIRECT_IN)
				printf("REDIRECT_IN\n");
			else if (current_redir->token == REDIRECT_OUT)
				printf("REDIRECT_OUT\n");
			else if (current_redir->token == REDIRECT_APPEND)
				printf("REDIRECT_APPEND\n");
			else if (current_redir->token == REDIRECT_HEREDOC)
				printf("REDIRECT_HEREDOC\n");
			
			// If it's a heredoc and we have a heredoc file, print its content
			if (current_redir->token == REDIRECT_HEREDOC && current_cmd->hd_file_name)
				print_heredoc_content(current_cmd->hd_file_name);
			else
				printf("redirection file: %s\n", current_redir->next->str);
			
			current_redir = current_redir->next->next;
		}
		printf("--------------\n");
		current_cmd = current_cmd->next;
	}
}

/**
 * Dolar işareti (değişken) işlemlerini gerçekleştirir.
 * Bu fonksiyon, metin içinde $ karakteri bulunduğunda çağrılır ve
 * değişken adını değişken değeriyle değiştirir veya
 * alfasayısal olmayan dolar işaretlerini işler.
 * 
 * @param result İşlenecek metin
 * @param i Metin içindeki konum referansı
 * @param vars Değişken listesi
 * @return İşlenmiş metin
 */
static char	*process_dollar(char *result, int *i, t_vars **vars)
{
	if (result[*i + 1] && (ft_isalpha(result[*i + 1]) || ft_isdigit(result[*i + 1])))
	{
		result = replace_env_var(result, *i, vars);
		*i = -1;
	}
	else
		result = handle_non_alpha_dollar(result, i);
	return (result);
}

/**
 * Bir metindeki tüm dolar işaretlerini işler.
 * Bu fonksiyon, verilen metni tarar ve $ karakteri
 * bulduğunda process_dollar fonksiyonunu çağırarak	
 * değişken değerleriyle değiştirir.
 * 
 * @param str İşlenecek metin
 * @param vars Değişken listesi
 * @return Tüm değişken referansları çözümlenmiş metin
 */
char	*is_dolar(char *str, t_vars **vars)
{
	int		i;
	char	*result;

	result = ft_strdup(str);
	free(str);
	i = 0;
	while (result[i])
	{
		if (result[i] == '$')
			result = process_dollar(result, &i, vars);
		i++;
	}
	return (result);
}

/**
 * Geçerli token'ı tipine göre işler.
 * Bu fonksiyon, token tipine göre uygun işlem fonksiyonunu çağırır:
 * - WORD token'ları için değişken işleme
 * - PIPE token'ları için pipe işleme
 * - Son token için komut oluşturma
 * 
 * @param current İşlenecek token'ın referansı
 * @param start Geçerli komutun başlangıç token'ı
 * @param cmd_list Komut listesi
 * @param vars Değişken listesi
 */
void	handle_current_token(t_lexer **current, t_lexer **start,
		t_simple_cmds **cmd_list, t_vars **vars)
{
	if ((*current)->token == WORD)
		handle_word_token(*current, vars);
	if ((*current)->token == PIPE)
		handle_pipe_token(current, start, cmd_list);
	else if (!(*current)->next)
		handle_last_token(start, (*current)->next, cmd_list);
}

/**
 * Token listesinden komut yapılarını oluşturur.
 * Bu fonksiyon, lexer analizi sonucu oluşturulan token listesini alır
 * ve token'ları komut yapılarına dönüştürür. Oluşturulan komutları
 * yazdırır ve sonrasında belleği serbest bırakır.
 * 
 * @param token_list Lexer analizi sonucu oluşturulan token listesi
 * @param vars Değişken listesi
 */
void	parse_commands(t_lexer *token_list, t_vars **vars)
{
	t_simple_cmds	*cmd_list;
	t_lexer			*start;
	t_lexer			*current;


	cmd_list = NULL;
	start = token_list;
	current = token_list;
	while (current)
	{
		handle_current_token(&current, &start, &cmd_list, vars);
		current = current->next;
	}
	// builtin_control(cmd_list, env);
	//Tam olarak burada executer fonksiyonunu çağırılacak
	// while (cmd_list)
	// 	cmd_list = cmd_list->next;
	// add_static_var(vars, "?", ft_itoa(cmd_list->return_value));

	execute(cmd_list);
	print_cmd_list(cmd_list);
	free_command_list(cmd_list);
}

/**
 * Girilen komutu tam olarak ayrıştırır.
 * Bu fonksiyon, kullanıcı tarafından girilen ham komutu alır,
 * önce lexer ile token'lara ayırır, ardından parse_commands ile
 * komut yapıları oluşturur ve sonunda belleği temizler.
 * 
 * @param input Kullanıcı tarafından girilen komut metni
 * @param vars Değişken listesi
 */
void	parser(char *input, t_vars **vars)
{
	t_lexer	*token_list;

	add_static_var(vars, "0", "minishell");
	token_list = lexer(input);
	if (!token_list)
		return ;
	parse_commands(token_list, vars);
	free_lexer_list(token_list);
}
