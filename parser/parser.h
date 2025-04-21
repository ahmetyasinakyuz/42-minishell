/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 18:13:27 by aycami            #+#    #+#             */
/*   Updated: 2025/04/21 18:13:30 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "../structs.h"
# include <stdio.h>

# define QUOTE_ERROR 1
# define SUCCESS 0

void			parser(char *input, t_vars **vars);
t_lexer			*lexer(char *input);
t_lexer			*create_token(char *str, t_tokens token_type, int index);
void			add_token(t_lexer **list, t_lexer *new_token);
t_tokens		check_token_type(char *str);
void			free_lexer_list(t_lexer *list);
t_simple_cmds	*create_command(t_lexer *start, t_lexer *end);
void			add_command(t_simple_cmds **cmd_list, t_simple_cmds *new_cmd);
void			parse_commands(t_lexer *token_list, t_vars **vars);
void			free_command_list(t_simple_cmds *list);
void			handle_redirections(t_simple_cmds *cmd, t_lexer **token_list);
void			add_redirection(t_lexer **redirection_list, t_lexer *token);
void			remove_token(t_lexer **list, t_lexer *token);
int				is_redirection(t_tokens token);
int				validate_quotes(char *str);
int				handle_quoted_string(char *str, int *i, char quote);
char			*extract_token(char *input, int *i);
char			*remove_quotes(char *str);
t_lexer			*copy_token(t_lexer *token);
int				is_in_vars(char *key, t_vars **vars);
char			*get_var(char *key, t_vars **vars);
char			*is_dolar(char *str, t_vars **vars);
char			*replace_env_var(char *result, int i, t_vars **vars);
void			add_var(t_vars **vars, char *key, char *value);
void			found_var(char *str, t_vars **vars);
void			clear_vars(t_vars **vars);
int				init_cmd(t_simple_cmds **cmd);
void			fill_words(t_simple_cmds *cmd, t_lexer *start, t_lexer *end);
int				count_words(t_lexer *start, t_lexer *end);
int				is_flag(char *str);
int				count_flags(t_lexer *start, t_lexer *end);
void			fill_flags(t_simple_cmds *cmd, t_lexer *start, t_lexer *end);
char			*handle_dollar_char(char *result, int *i, t_vars **vars);
char			*remove_quotes(char *str);
int				handle_quoted_string(char *str, int *i, char quote);
void			handle_quote_status(char *input, int *i, int *in_squote,
					int *in_dquote);
void			process_quotes(char *str, char *result, int *i, int *j);
void			handle_word_token(t_lexer *current, t_vars **vars);
void			handle_pipe_token(t_lexer **current, t_lexer **start,
					t_simple_cmds **cmd_list);
void			handle_last_token(t_lexer **start, t_lexer *end,
					t_simple_cmds **cmd_list);
char			*handle_non_alpha_dollar(char *result, int *i);
void			add_static_var(t_vars **vars, char *key, char *value);
char			*create_heredoc_file(char *delimiter);
void			handle_heredoc(t_simple_cmds *cmd, t_lexer *redirections);
void			process_heredoc_input(int fd, char *delimiter);

#endif
