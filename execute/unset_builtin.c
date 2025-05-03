/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:40:30 by codespace         #+#    #+#             */
/*   Updated: 2025/05/03 19:29:15 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**filter_env_without_vars(t_simple_cmds *cmd_list, char **envp)
{
	int		i[3];
	char	**new_env;
	int		env_len;
	int		len;

	i[0] = 0;
	i[2] = 0;
	env_len = 0;
	while (envp && envp[env_len])
		env_len++;
	new_env = malloc(sizeof(char *) * (env_len + 1));
	if (!new_env)
		return (NULL);
	while (envp[i[0]])
	{
		i[1] = 1;
		while (cmd_list->str[i[1]])
		{
			len = ft_strlen(cmd_list->str[i[1]]);
			if (ft_strncmp(envp[i[0]], cmd_list->str[i[1]], len) == 0
				&& envp[i[0]][len] == '=')
				break ;
			i[1]++;
		}
		if (!cmd_list->str[i[1]])
			new_env[i[2]++] = ft_strdup(envp[i[0]]);
		i[0]++;
	}
	new_env[i[2]] = NULL;
	return (new_env);
}

void	unset_builtin(t_simple_cmds *cmd_list, char ***envp)
{
	char	**new_env;
	int		i;

	if (cmd_list->flag)
	{
		printf("This command only works without the flag.\n");
		cmd_list->return_value = 1;
		return ;
	}
	new_env = filter_env_without_vars(cmd_list, *envp);
	if (!new_env)
	{
		perror("malloc");
		exit(1);
	}
	i = 0;
	while ((*envp)[i])
	{
		free((*envp)[i]);
		i++;
	}
	free(*envp);
	*envp = new_env;
	cmd_list->return_value = 0;
}
