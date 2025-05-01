/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:40:27 by codespace         #+#    #+#             */
/*   Updated: 2025/05/01 14:40:28 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**new_env_maker(char ***envp, int extra)
{
	char **new;
	int len;
	int i;

	if (extra <= 0)
		return (NULL);

	len = 0;

	while (envp && (*envp)[len])
		len++;

	new = malloc(sizeof(char *) * (len + extra + 1));

	if (!new)
		return (NULL);
	i = 0;

	// Duplicate existing env strings
	while (i < len)
	{
		new[i] = ft_strdup((*envp)[i]);
		i++;
	}

	// Initialize extra slots
	while (i < len + extra)
	{
		new[i] = NULL;
		i++;
	}

	new[i] = NULL;
	return (new);
}

void	export_builtin(t_simple_cmds *cmd_list, char ***envp)
{
	int i;
	int j;
	int len;

	i = 0;
	j = 0;

	if (cmd_list->flag)
	{
		printf("This command only works without the flag.\n");
		cmd_list->return_value = 1;
		return;
	}

	while (cmd_list->str[j])
		j++;

	len = 0;

	while ((*envp) && (*envp)[len])
		len++;

	*envp = new_env_maker(envp, j);

	// printf("YENİ ENVP:");
	// i = 0;
	// while (*envp[i])
	//{
	// printf("%s\n", *envp[i]);
	// i++;
	//}

	printf("YENİ ENV OLUŞTURULDU\n");

	if (!(*envp))
	{
		perror("malloc");
		exit(1);
	}

	j = 1;

	while (cmd_list->str[j])
	{
		(*envp)[len] = ft_strdup(cmd_list->str[j]);
		j++;
		len++;
	}

	(*envp)[len] = NULL;
	i = 0;

	while ((*envp)[i])
	{
		printf("%s\n", (*envp)[i]);
		i++;
	}

	cmd_list->return_value = 0;
}
