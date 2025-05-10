/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 12:24:26 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/10 12:30:16 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

pid_t	parse_ps_output(char *buffer)
{
	int		i;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i])
		i++;
	return (process_ps_lines(buffer, i));
}

void	execute_ps(int pipe_fd[2])
{
	char	*ps_argv[4];

	close(pipe_fd[0]);
	dup2(pipe_fd[1], STDOUT_FILENO);
	close(pipe_fd[1]);
	ps_argv[0] = "ps";
	ps_argv[1] = "-o";
	ps_argv[2] = "pid,ppid,command";
	ps_argv[3] = NULL;
	execve("/bin/ps", ps_argv, NULL);
	exit(1);
}

pid_t	get_shell_pid(void)
{
	int		pipe_fd[2];
	pid_t	child_pid;
	int		status;
	char	buffer[4096];
	int		bytes_read;

	if (pipe(pipe_fd) == -1)
		return (0);
	child_pid = fork();
	if (child_pid == -1)
	{
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		return (0);
	}
	if (child_pid == 0)
		execute_ps(pipe_fd);
	close(pipe_fd[1]);
	bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
	close(pipe_fd[0]);
	if (bytes_read <= 0)
		return (0);
	buffer[bytes_read] = '\0';
	waitpid(child_pid, &status, 0);
	return (parse_ps_output(buffer));
}

int	count_digits(pid_t pid)
{
	int		digits;
	pid_t	temp;

	digits = 1;
	temp = pid;
	while (temp >= 10)
	{
		digits++;
		temp /= 10;
	}
	return (digits);
}

char	*pid_to_string(pid_t pid)
{
	char	*str;
	int		digits;
	int		i;

	digits = count_digits(pid);
	str = malloc(digits + 1);
	if (!str)
		return (NULL);
	str[digits] = '\0';
	i = digits - 1;
	while (i >= 0)
	{
		str[i] = '0' + (pid % 10);
		pid /= 10;
		i--;
	}
	return (str);
}
