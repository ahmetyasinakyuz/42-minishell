/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:46 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/11 12:54:25 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_tokens
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_HEREDOC,
	REDIRECT_OUT,
	REDIRECT_APPEND,
	EMPTY,
}								t_tokens;

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
}								t_io_type;

typedef struct s_vars
{
	char						*key;
	char						*value;
	struct s_vars				*next;
	struct s_vars				*prev;
}								t_vars;

typedef struct s_lexer
{
	char						*str;
	t_tokens					token;
	int							i;
	struct s_lexer				*next;
	struct s_lexer				*prev;
}								t_lexer;

typedef struct s_simple_cmds
{
	char						**content;
	char						**str;
	char						**flag;
	char						**env;
	char						*hd_file_name;
	int							num_redirections;
	int							pipe;
	int							return_value;
	int							input_fd;
	int							output_fd;
	t_lexer						*redirections;
	t_io_type					input_type;
	t_io_type					output_type;
	struct s_simple_cmds		*next;
	struct s_simple_cmds		*prev;
}								t_simple_cmds;

typedef struct s_free
{
	char						**envp;
	t_lexer						*token_list;
	pid_t						*pids;
	t_vars						**vars;
	t_simple_cmds				*cmd_list;
}								t_free;

typedef struct s_context
{
	char						***envp;
	t_lexer						*token_list;
	t_vars						**vars;
}								t_context;

typedef struct s_exec_state
{
	t_simple_cmds				*current_cmd;
	t_simple_cmds				*last_cmd;
	pid_t						*pids;
	int							cmd_count;
	int							i;
	int							status;
}								t_exec_state;

typedef struct s_exec_child_ctx
{
	char						***envp;
	t_lexer						*token_list;
	pid_t						*pids;
	t_vars						**vars;
}								t_exec_child_ctx;

typedef struct s_exitb_ctx
{
	char						**envp;
	t_lexer						*token_list;
	pid_t						*pids;
	t_vars						**vars;
}								t_exitb_ctx;

typedef struct s_builtin_ctx
{
	char						***envp;
	t_lexer						*token_list;
	pid_t						*pids;
	t_vars						**vars;
}								t_builtin_ctx;

typedef struct s_fork_exec_ctx
{
	t_simple_cmds				*cmd_list;
	char						***envp;
	t_lexer						*token_list;
	t_vars						**vars;
}								t_fork_exec_ctx;

typedef struct s_handle_ctx
{
	t_exec_state				*st;
	char						***envp;
	t_lexer						*token_list;
	t_vars						**vars;
}								t_handle_ctx;

typedef struct s_cleanup_ctx
{
	char						**envp;
	t_lexer						*token_list;
	pid_t						*pids;
	t_vars						**vars;
	int							code;
}								t_cleanup_ctx;

extern volatile sig_atomic_t	g_received_signal;

void							run_shell(t_vars **vars, char ***envp);
void							setup_signals(void);
void							setup_execute_signals(void);
void							setup_heredoc_signals(void);
void							setup_continuation_signals(void);
void							reset_signal_handling(void);
void							handle_sigint(int signum);
void							setup_child_signals(void);
char							*process_dollar(char *result, int *i,
									t_vars **vars);
void							parser(char *input, t_vars **vars,
									char ***envp);
t_lexer							*lexer(char *input);
t_lexer							*create_token(char *str, t_tokens token_type,
									int index);
void							add_token(t_lexer **list, t_lexer *new_token);
t_tokens						check_token_type(char *str);
void							free_lexer_list(t_lexer *list);
t_simple_cmds					*create_command(t_lexer *start, t_lexer *end,
									t_vars vars);
t_simple_cmds					*setup_command_flags(t_simple_cmds *cmd,
									t_lexer *start, t_lexer *end, t_vars vars);
t_simple_cmds					*setup_command_str(t_simple_cmds *cmd,
									t_lexer *start, t_lexer *end);
t_simple_cmds					*setup_command_content(t_simple_cmds *cmd,
									t_lexer *start, t_lexer *end);
void							add_command(t_simple_cmds **cmd_list,
									t_simple_cmds *new_cmd);
void							parse_commands(t_lexer *token_list,
									t_vars **vars, char ***envp);
void							free_command_list(t_simple_cmds *list);
void							handle_redirections(t_simple_cmds *cmd,
									t_lexer **token_list);
void							add_redirection(t_lexer **redirection_list,
									t_lexer *token);
int								is_redirection(t_tokens token);
int								validate_quotes(char *str);
int								handle_quoted_string(char *str, int *i,
									char quote);
char							*extract_token(char *input, int *i);
char							*remove_quotes(char *str);
t_lexer							*copy_token(t_lexer *token);
int								is_in_vars(char *key, t_vars **vars);
char							*get_var(char *key, t_vars **vars);
char							*is_dolar(char *str, t_vars **vars);
char							*replace_env_var(char *result, int i,
									t_vars **vars);
void							add_var(t_vars **vars, char *key, char *value);
void							found_var(char *str, t_vars **vars);
void							clear_vars(t_vars **vars);
int								init_cmd(t_simple_cmds **cmd);
void							fill_words(t_simple_cmds *cmd, t_lexer *start,
									t_lexer *end);
int								count_words(t_lexer *start, t_lexer *end);
int								is_flag(char *str);
int								count_flags(t_lexer *start, t_lexer *end);
void							fill_flags(t_simple_cmds *cmd, t_lexer *start,
									t_lexer *end);
char							*handle_dollar_char(char *result, int *i,
									t_vars **vars);
char							*remove_quotes(char *str);
void							process_quotes(char *str, char *result, int *i,
									int *j);
void							handle_word_token(t_lexer *current,
									t_vars **vars);
void							handle_pipe_token(t_lexer **current,
									t_lexer **start, t_simple_cmds **cmd_list,
									t_vars vars);
void							handle_last_token(t_lexer **start, t_lexer *end,
									t_simple_cmds **cmd_list, t_vars vars);
void							add_static_var(t_vars **vars, char *key,
									char *value);
int								update_existing_var(t_vars **vars, char *key,
									char *value);
t_vars							*create_var_node(char *key, char *value);
char							*create_heredoc_file(char *delimiter,
									t_vars *vars);
void							handle_heredoc(t_simple_cmds *cmd,
									t_lexer *redirections, t_vars *vars);
void							process_heredoc_input(int fd, char *delimiter,
									t_vars *vars);
void							process_single_heredoc(t_simple_cmds *cmd,
									t_lexer *current, t_vars *vars);
char							*get_env_value(char *env_var, t_vars **vars);
int								handle_pipe_char(char *token, char *input,
									int *i, int *j);
int								handle_redirect_char(char *token, char *input,
									int *i, int *j);
void							handle_quotes(char c, int *in_squote,
									int *in_dquote);
void							free_str_array(char **arr);
void							free_command_content(t_simple_cmds *cmd);
char							*remove_whitespaces(char *str);
void							execute(t_simple_cmds *cmd_list, char ***envp,
									t_lexer *token_list, t_vars **vars);
void							builtin_control(t_simple_cmds *cmd_list,
									t_builtin_ctx *ctx);
void							echo_builtin(t_simple_cmds *cmd_list);
char							*path_finder(char *cmd, char **envp);
void							none_built_in(t_simple_cmds *cmd_list,
									char ***envp);
void							free_paths(char **paths);
void							io_handle(t_simple_cmds *cmd_list);
void							env_builtin(t_simple_cmds *cmd_list,
									char ***envp);
void							export_builtin(t_simple_cmds *cmd_list,
									char ***envp, int flag);
void							empty_export(char ***envp);
void							unset_builtin(t_simple_cmds *cmd_list,
									char ***envp);
void							pwd_builtin(t_simple_cmds *cmd_list);
void							cd_builtin(t_simple_cmds *cmd_list,
									char **envp);
void							exit_builtin(t_simple_cmds *cmd_list,
									t_exitb_ctx *ctx);
int								ft_new_atoi(const char *str, int *flag);
void							free_env(char **env);
char							**new_env_maker(char ***envp, int extra);
char							*get_env_key(const char *env_str);
int								find_env_index(char **envp, const char *key);
int								is_valid_identifier(const char *s);
void							export_with_flag_error(t_simple_cmds *cmd_list);
void							print_invalid_identifier(char *str);
void							update_env_entry(char **envp, int idx,
									char *new_entry);
char							**allocate_merged_array(char **cmd,
									char **flags);
char							**merge_cmd_and_flags(char **cmd, char **flags);
int								is_directory(const char *path);
void							print_cmd_not_found(char *cmd, int *ret);
void							handle_command_error(t_simple_cmds *cmd_list,
									char *cmd, char *path);
int								is_invalid_command(t_simple_cmds *cmd_list);
void							init_exec_state(t_exec_state *st,
									t_simple_cmds *cmd_list);
void							fork_and_execute(t_exec_state *st,
									t_fork_exec_ctx *ctx);
void							wait_for_children(t_exec_state *st);
void							cleanup(t_simple_cmds *cmd_list,
									t_cleanup_ctx *ctx);
int								is_direct_path(char *str);
char							*resolve_command_path(t_simple_cmds *cmd_list,
									char **envp, int *allocated);
char							**prepare_cmd(t_simple_cmds *cmd_list,
									int *cmd_allocated);
void							i_handle(t_simple_cmds *cmd_list);
void							o_handle(t_simple_cmds *cmd_list);
int								handle_file_error(t_simple_cmds *cmd_list,
									char *filename, char *error_msg);
char							*unquote_filename(char *str);
void							clean_exit(t_vars **vars, char ***envp);
int								is_quote_closed(char *input);
int								is_input_incomplete(char *input);
char							*get_continuation(char *input);
void							run_shell(t_vars **vars, char ***envp);
int								handle_null_input(t_vars **vars, char ***envp);
void							handle_input_loop(char *input, t_vars **vars,
									char ***envp);
char							**env_maker(char **envp);
int								validate_redirections(t_lexer *token_list);
char							*join_env_parts(char *result, int i,
									char *env_value, int j);
pid_t							get_shell_pid(void);
char							*pid_to_string(pid_t pid);
pid_t							process_ps_lines(char *buffer, int i);
void							found_var(char *str, t_vars **vars);
char							*get_env_value(char *env_var, t_vars **vars);
char							*get_pid_value(void);
char							*process_question_mark(char *result, int *i,
									t_vars **vars);
int								open_heredoc_file(char **filename);
void							cleanup_on_interrupt(int fd, char *filename,
									int original_stdin);
void							restore_stdin(int original_stdin);
char							*process_dollar_sign(char *result, int *i,
									t_vars **vars);
int								setup_heredoc_signals_and_save(void);
void							restore_heredoc_signals(int original_signal);
void							handle_heredoc_sigint(int signum);
void							handle_sigint(int signum);
void							handle_continuation_sigint(int signum);
void							setup_signals(void);
char							*generate_temp_filename(void);
void							restore_heredoc_signals(int original_signal);
void							setup_child_signals(void);
void							setup_execute_signals(void);
void							setup_heredoc_signals(void);
void							setup_continuation_signals(void);
void							reset_signal_handling(void);
int	handle_heredoc_line(char *line, char *delimiter, t_vars *vars,
	char ***buffer);
void	write_buffer_to_fd(int fd, char **buffer, int sigint_received);
#endif
