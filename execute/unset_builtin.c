/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:40:30 by codespace         #+#    #+#             */
/*   Updated: 2025/05/01 14:53:59 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**filter_env_without_vars(t_simple_cmds *cmd_list, char **envp)
{
	int		i = 0;
	int		j;
	int		k = 0;
	char	**new_env;
	int		env_len = 0;

	while (envp && envp[env_len])
		env_len++;

	new_env = malloc(sizeof(char *) * (env_len + 1));
	if (!new_env)
		return (NULL);

	while (envp[i])
	{
		j = 1;
		while (cmd_list->str[j])
		{
			int len = ft_strlen(cmd_list->str[j]);
			if (ft_strncmp(envp[i], cmd_list->str[j], len) == 0
				&& envp[i][len] == '=')
				break;
			j++;
		}
		if (!cmd_list->str[j])
			new_env[k++] = ft_strdup(envp[i]);
		i++;
	}
	new_env[k] = NULL;
	return (new_env);
}

void	unset_builtin(t_simple_cmds *cmd_list, char ***envp)
{
	char	**new_env;

	if (cmd_list->flag)
	{
		printf("This command only works without the flag.\n");
		cmd_list->return_value = 1;
		return;
	}

	new_env = filter_env_without_vars(cmd_list, *envp);

	if (!new_env)
	{
		perror("malloc");
		exit(1);
	}

	// Free old env
	int i = 0;
	while ((*envp)[i])
	{
		free((*envp)[i]);
		i++;
	}
	free(*envp);

	*envp = new_env;
	cmd_list->return_value = 0;
}
