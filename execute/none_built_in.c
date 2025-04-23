/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   none_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/04/23 17:16:53 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**merge_cmd_and_flags(char **cmd, char **flags)
{
	int		i;
	int		j;
	char	**merged;

	i = 0;
	j = 0;
	while (cmd[i])
		i++;
	while (flags[j])
		j++;
	merged = malloc(sizeof(char *) * (i + j + 1));
	if (!merged)
		return (NULL);
	i = 0;
	j = 0;
	while (cmd[i])
	{
		merged[i] = cmd[i];
		i++;
	}
	while (flags[j])
	{
		merged[i] = flags[j];
		i++;
		j++;
	}
	merged[i] = NULL;
	return (merged);
}

void none_built_in(t_simple_cmds *cmd_list, char **envp)
{
	char	*path;
	char	**cmd;
	int		merged_alloc = 0;
	int		should_free_path = 0;

	if(cmd_list->str[0] == NULL)
	{
		cmd_list->return_value = 127;
		return ;
	}
	if (cmd_list->str[0][0] == '/' ||
		(cmd_list->str[0][0] == '.' && cmd_list->str[0][1] == '/') ||
		(cmd_list->str[0][0] == '.' && cmd_list->str[0][1] == '.' && cmd_list->str[0][2] == '/'))
	{
		path = cmd_list->str[0];
		should_free_path = 0;
	}
	else
	{
		path = path_finder(cmd_list->str[0], envp);
		should_free_path = 1;
	}
	if (path == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_list->str[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cmd_list->return_value = 127;
		return ;
	}
	if(cmd_list->flag == NULL)
		cmd = cmd_list->str;
	else
	{
		cmd = merge_cmd_and_flags(cmd_list->str, cmd_list->flag);
		merged_alloc = 1;
	}
	pid_t pid = fork();
	if (pid == 0)
	{
		execve(path, cmd, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
		cmd_list->return_value = 1;
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
		{
			cmd_list->return_value = WEXITSTATUS(status);
			printf("Child process exited with status: %d\n", cmd_list->return_value);
		}
		else if (WIFSIGNALED(status))
		{
			cmd_list->return_value = 128 + WTERMSIG(status);
			printf("Child process exited by signal: %d (return_value: %d)\n", WTERMSIG(status), cmd_list->return_value);
		}
	}
	if (should_free_path)
		free(path);
	if (merged_alloc)
		free(cmd);
	printf("Child process exited with status: %d\n", cmd_list->return_value);
}
