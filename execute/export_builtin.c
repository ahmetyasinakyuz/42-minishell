/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:40:27 by codespace         #+#    #+#             */
/*   Updated: 2025/05/02 16:54:48 by aycami           ###   ########.fr       */
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
	}
	while (cmd_list->str[j])
		j++;

	len = 0;
	while ((*envp) && (*envp)[len])
		len++;
	*envp = new_env_maker(envp, j);
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
	cmd_list->return_value = 0;
}
