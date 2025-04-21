/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:11:16 by aycami            #+#    #+#             */
/*   Updated: 2025/04/21 18:19:49 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include "libs/ft_printf/ft_printf.h"
# include "libs/get_next_line/get_next_line.h"
# include "libs/libft/libft.h"

typedef enum e_tokens
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_HEREDOC,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	EMPTY,
}							t_tokens;

typedef enum e_io_type
{
	IO_STDIN,
	IO_STDOUT,
	IO_PIPE_IN,
	IO_PIPE_OUT,
	IO_FILE_IN,
	IO_FILE_OUT,
	IO_HEREDOC,
	IO_APPEND,
}							t_io_type;

typedef struct s_vars
{
	char					*key;
	char					*value;
	struct s_vars			*next;
	struct s_vars			*prev;
}							t_vars;

typedef struct s_lexer
{
	char					*str;
	t_tokens				token;
	int						i;
	struct s_lexer			*next;
	struct s_lexer			*prev;
}							t_lexer;

typedef struct s_simple_cmds
{
	char					**str;
	char					**flag;
	int						num_redirections;
	char					*hd_file_name;
	int						pipe;
	t_lexer					*redirections;
	t_io_type				input_type;
	t_io_type				output_type;
	int						return_value;
	int						input_fd;
	int						output_fd;
	char					*raw_command;
	char					**env;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}						t_simple_cmds;

#endif