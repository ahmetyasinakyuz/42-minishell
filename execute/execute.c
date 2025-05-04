/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 17:09:59 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 14:21:55 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_env(char **env)
{
	int	i;

	i = 0;
	if (!env)
		return ;
	while (env[i])
	{
		free(env[i]);
		i++;
	}
	free(env);
}

void	handle_pipe(t_simple_cmds *cmd, t_simple_cmds *next)
{
	int	pipe_fd[2];

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		exit(1);
	}
	cmd->output_fd = pipe_fd[1];
	cmd->output_type = IO_PIPE_OUT;
	cmd->pipe = 1;
	next->input_fd = pipe_fd[0];
	next->input_type = IO_PIPE_IN;
	next->pipe = 1;
}

void	count_commands(t_simple_cmds *cmd_list, int *cmd_count, t_simple_cmds **last_cmd)
{
	*cmd_count = 0;
	while (cmd_list)
	{
		(*cmd_count)++;
		*last_cmd = cmd_list;
		cmd_list = cmd_list->next;
	}
}

int	init_execute_struct(t_free *free_struct, pid_t **pids, int cmd_count,
						t_lexer *token_list, t_vars **vars, char ***envp)
{
	int	i;

	*pids = malloc(sizeof(pid_t) * cmd_count);
	if (!*pids)
	{
		perror("malloc");
		return (0);
	}
	i = 0;
	while (i < cmd_count)
		(*pids)[i++] = -1;
	free_struct->envp = *envp;
	free_struct->token_list = token_list;
	free_struct->pids = *pids;
	free_struct->vars = vars;
	return (1);
}

void	run_single_command(t_simple_cmds *cmd, t_free *free_struct, pid_t *pids, int i)
{
	if (cmd->next)
		handle_pipe(cmd, cmd->next);
	pids[i] = fork();
	if (pids[i] == -1)
	{
		perror("fork");
		exit(1);
	}
	if (pids[i] == 0)
	{
		setup_child_signals();
		io_handle(cmd);
		builtin_control(cmd, free_struct);
	}
	if (cmd->output_type == IO_PIPE_OUT)
		close(cmd->output_fd);
	if (cmd->input_type == IO_PIPE_IN)
		close(cmd->input_fd);
}

void	wait_for_children(pid_t *pids, int cmd_count, t_simple_cmds *cmd_list)
{
	int	i;
	int	status;
	t_simple_cmds *current_cmd;

	i = 0;
	current_cmd = cmd_list;
	while (i < cmd_count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (WIFSIGNALED(status))
			{
				if (WTERMSIG(status) == SIGQUIT)
					write(STDERR_FILENO, "Quit (core dumped)\n", 19);
				else if (WTERMSIG(status) == SIGINT)
					write(STDOUT_FILENO, "\n", 1);
				current_cmd->return_value = 128 + WTERMSIG(status);
			}
			else if (WIFEXITED(status))
				current_cmd->return_value = WEXITSTATUS(status);
		}
		if (current_cmd && current_cmd->next)
			current_cmd = current_cmd->next;
		i++;
	}
}

static int handle_builtin_commands(t_exec_state *state)
{
	char *cmd = *state->current_cmd->str;
	int is_pipeline = (state->current_cmd->pipe || 
		(state->current_cmd->next != NULL || state->current_cmd->prev != NULL));

	if (is_pipeline && (ft_strncmp("export", cmd, 7) == 0 || 
						ft_strncmp("unset", cmd, 6) == 0 || 
						ft_strncmp("cd", cmd, 3) == 0))
	{
		pid_t pid = fork();
		if (pid == -1)
		{
			perror("fork");
			return (1);
		}
		if (pid == 0)
		{
			setup_child_signals();
			io_handle(state->current_cmd);
			if (ft_strncmp("export", cmd, 7) == 0)
				export_builtin(state->current_cmd, &state->free_struct.envp);
			else if (ft_strncmp("unset", cmd, 6) == 0)
				unset_builtin(state->current_cmd, &state->free_struct.envp);
			else if (ft_strncmp("cd", cmd, 3) == 0)
				cd_builtin(state->current_cmd, state->free_struct.envp);
			exit(state->current_cmd->return_value);
		}
		state->pids[state->i++] = pid;
		if (state->current_cmd->output_type == IO_PIPE_OUT)
			close(state->current_cmd->output_fd);
		if (state->current_cmd->input_type == IO_PIPE_IN)
			close(state->current_cmd->input_fd);
		return (1);
	}
	else if (ft_strncmp("export", cmd, 7) == 0)
		export_builtin(state->current_cmd, &state->free_struct.envp);
	else if (ft_strncmp("unset", cmd, 6) == 0)
		unset_builtin(state->current_cmd, &state->free_struct.envp);
	else if (ft_strncmp("cd", cmd, 3) == 0)
		cd_builtin(state->current_cmd, state->free_struct.envp);
	else if (ft_strncmp("echo", cmd, 5) == 0 &&
		!state->current_cmd->next && !state->current_cmd->prev)
		echo_builtin(state->current_cmd);
	else if (ft_strncmp("pwd", cmd, 4) == 0 &&
		!state->current_cmd->next && !state->current_cmd->prev)
		pwd_builtin(state->current_cmd);
	else if (ft_strncmp("env", cmd, 4) == 0 &&
		!state->current_cmd->next && !state->current_cmd->prev)
		env_builtin(state->current_cmd, state->free_struct.envp);
	else if (ft_strncmp("exit", cmd, 5) == 0)
	{
		state->free_struct.token_list = state->free_struct.token_list;
		state->free_struct.pids = state->pids;
		state->free_struct.vars = state->free_struct.vars;
		exit_builtin(state->current_cmd, &state->free_struct);
	}
	else
		return (0);
	return (1);
}

static void execute_loop(t_exec_state *state)
{
	while (state->current_cmd)
	{
		if (!handle_builtin_commands(state))
			run_single_command(state->current_cmd, &state->free_struct, state->pids, state->i++);
		state->current_cmd = state->current_cmd->next;
	}
}

static int init_execute_state(t_exec_state *state, t_simple_cmds *cmd_list, t_context *ctx)
{
	count_commands(cmd_list, &state->cmd_count, &state->last_cmd);
	if (!init_execute_struct(&state->free_struct, &state->pids,
		state->cmd_count, ctx->token_list, ctx->vars, ctx->envp))
		return (0);
	setup_execute_signals();
	state->free_struct.cmd_list = cmd_list;
	state->current_cmd = cmd_list;
	state->i = 0;
	return (1);
}

void execute(t_simple_cmds *cmd_list, char ***envp, t_lexer *token_list, t_vars **vars)
{
	t_exec_state state;
	t_context ctx;

	ctx.envp = envp;
	ctx.token_list = token_list;
	ctx.vars = vars;

	if (!init_execute_state(&state, cmd_list, &ctx))
		return ;
	execute_loop(&state);
	wait_for_children(state.pids, state.i, cmd_list);
	setup_signals();
	free(state.pids);
}
