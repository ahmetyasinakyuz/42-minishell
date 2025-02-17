/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:19 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/17 19:07:12 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../libft/libft.h"
# include <stdio.h>

typedef enum e_tokens
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_HEREDOC,
	REDIRECT_OUT,
	REDIRECT_APPEND,
}							t_tokens;
typedef struct s_lexer
{
	char					*str;
	t_tokens				token;
	int						i;
	struct s_lexer			*next;
	struct s_lexer			*prev;
}							t_lexer;
typedef struct s_tools
{
	int						placeholder;
}							t_tools;

typedef struct s_simple_cmds
{
	char					**str;
	int						(*builtin)(t_tools *, struct s_simple_cmds *);
	int						num_redirections;
	char					*hd_file_name;
	t_lexer					*redirections;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}							t_simple_cmds;

void						parser(char *input);
t_lexer						*lexer(char *input);
t_lexer						*create_token(char *str, t_tokens token_type,
								int index);
void						add_token(t_lexer **list, t_lexer *new_token);
t_tokens					check_token_type(char *str);
void						free_lexer_list(t_lexer *list);
t_simple_cmds				*create_command(t_lexer *start, t_lexer *end);
void						add_command(t_simple_cmds **cmd_list,
								t_simple_cmds *new_cmd);
void						parse_commands(t_lexer *token_list);
void						free_command_list(t_simple_cmds *list);
void						handle_redirections(t_simple_cmds *cmd,
								t_lexer **token_list);
void						add_redirection(t_lexer **redirection_list,
								t_lexer *token);
void						remove_token(t_lexer **list, t_lexer *token);
int							is_redirection(t_tokens token);

#endif
