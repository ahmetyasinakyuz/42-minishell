/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:40:27 by codespace         #+#    #+#             */
/*   Updated: 2025/05/02 18:32:33 by aakyuz           ###   ########.fr       */
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
	while (i < len)
	{
		new[i] = ft_strdup((*envp)[i]);
		i++;
	}
	while (i < len + extra)
	{
		new[i] = NULL;
		i++;
	}
	new[i] = NULL;
	return (new);
}

void	empty_export(char ***envp)
{
	int i;

	i = 0;
	while((*envp)[i])
	{
		printf("declare -x %s\n", (*envp)[i]);
		i++;
	}
}

void	export_builtin(t_simple_cmds *cmd_list, char ***envp)
{
	int j;
	int len;
	char **new_env;

	j = 0;
	if (cmd_list->flag)
	{
		printf("This command only works without the flag.\n");
		cmd_list->return_value = 1;
		return;
	}
	if(!cmd_list->str[1])
	{
		empty_export(envp);
		cmd_list->return_value = 0;
		return;
	}
	while (cmd_list->str[j])
		j++;

	new_env = new_env_maker(envp, j - 1);
	if (!new_env)
	{
		perror("malloc");
		exit(1);
	}
	len = 0;
	while (new_env[len])
		len++;
	j = 1;
	while (cmd_list->str[j])
	{
		new_env[len] = ft_strdup(cmd_list->str[j]);
		j++;
		len++;
	}
	new_env[len] = NULL;
	
	// Free old environment
	free_env(*envp);
	*envp = new_env;
	cmd_list->return_value = 0;
}
