/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:46 by aakyuz            #+#    #+#             */
/*   Updated: 2025/04/23 16:20:00 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include "libs/ft_printf/ft_printf.h"
# include "libs/get_next_line/get_next_line.h"
# include "libs/libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <unistd.h>
# include <signal.h>
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <errno.h>

# define QUOTE_ERROR 1
# define SUCCESS 0

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
	char					**content;
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
	char					**env;
	struct s_simple_cmds	*next;
	struct s_simple_cmds	*prev;
}						t_simple_cmds;

extern volatile sig_atomic_t	g_received_signal;

//--------------------------ANA KISIM--------------------------------
void	run_shell(t_vars **vars, char **envp);
void	setup_signals(void);
void	reset_signal_handling(void);
void	handle_sigint(int signum);

//--------------------------PARSER-----------------------------------
void			parser(char *input, t_vars **vars, char **envp);
t_lexer			*lexer(char *input);
t_lexer			*create_token(char *str, t_tokens token_type, int index);
void			add_token(t_lexer **list, t_lexer *new_token);
t_tokens		check_token_type(char *str);
void			free_lexer_list(t_lexer *list);
t_simple_cmds	*create_command(t_lexer *start, t_lexer *end);
void			add_command(t_simple_cmds **cmd_list, t_simple_cmds *new_cmd);
void			parse_commands(t_lexer *token_list, t_vars **vars, char **envp);
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

//--------------------------EXECUTE-----------------------------------
void	execute(t_simple_cmds *cmd_list, char **envp);
void	builtin_control(t_simple_cmds *cmd_list, char **envp);
void	echo_builtin(t_simple_cmds *cmd_list);
char	*path_finder(char *cmd, char **envp);
void	none_built_in(t_simple_cmds *cmd_list, char **envp);
void	free_paths(char **paths);

#endif
