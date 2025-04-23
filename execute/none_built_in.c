/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   none_built_in.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/04/23 11:52:58 by aakyuz           ###   ########.fr       */
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

	path = path_finder(cmd_list->str[0] , envp);
	if (path == NULL)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd_list->str[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		cmd_list->return_value = 127;
		return ;
	}
	pid_t pid = fork();
	//cmd_list->str ile cmd_list->flagi birleştir ondan sonra execve'ye gönder
	cmd = merge_cmd_and_flags(cmd_list->str, cmd_list->flag);
	if (pid == 0)
	{
		execve(path, cmd, envp);
		perror("execve");
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else
	{
		int status;
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			cmd_list->return_value = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			cmd_list->return_value = 126 + WTERMSIG(status);
	}
}
