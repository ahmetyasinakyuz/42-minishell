/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:30:15 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/11 09:27:14 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_custom_strstr(const char *haystack, const char *needle)
{
	size_t	i;
	size_t	j;

	if (!*needle)
		return ((char *)haystack);
	i = 0;
	while (haystack[i])
	{
		j = 0;
		while (needle[j] && haystack[i + j] == needle[j])
			j++;
		if (!needle[j])
			return ((char *)(haystack + i));
		i++;
	}
	return (NULL);
}

void	extract_pid(char *buffer, int *i, char *pid_str, char *ppid_str)
{
	int	j;

	j = 0;
	while (buffer[*i] && (buffer[*i] == ' ' || buffer[*i] == '\t'))
		(*i)++;
	while (buffer[*i] && buffer[*i] != ' ' && buffer[*i] != '\t')
		pid_str[j++] = buffer[(*i)++];
	pid_str[j] = '\0';
	j = 0;
	while (buffer[*i] && (buffer[*i] == ' ' || buffer[*i] == '\t'))
		(*i)++;
	while (buffer[*i] && buffer[*i] != ' ' && buffer[*i] != '\t')
		ppid_str[j++] = buffer[(*i)++];
	ppid_str[j] = '\0';
}

void	extract_cmd(char *buffer, int *i, char *cmd)
{
	int	j;

	j = 0;
	while (buffer[*i] && (buffer[*i] == ' ' || buffer[*i] == '\t'))
		(*i)++;
	while (buffer[*i] && buffer[*i] != '\n' && j < 63)
		cmd[j++] = buffer[(*i)++];
	cmd[j] = '\0';
}

pid_t	check_for_minishell(char *pid_str, char *cmd)
{
	if (ft_custom_strstr(cmd, "minishell") && !ft_custom_strstr(cmd, "ps"))
		return (ft_atoi(pid_str));
	return (0);
}

pid_t	process_ps_lines(char *buffer, int i)
{
	char	pid_str[16];
	char	ppid_str[16];
	char	cmd[64];
	pid_t	pid;

	if (!buffer)
		return (0);
	while (buffer[i])
	{
		extract_pid(buffer, &i, pid_str, ppid_str);
		extract_cmd(buffer, &i, cmd);
		pid = check_for_minishell(pid_str, cmd);
		if (pid > 0)
			return (pid);
		while (buffer[i] && buffer[i] != '\n')
			i++;
		if (buffer[i])
			i++;
	}
	return (0);
}
