/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_builtin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:14:28 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 09:14:30 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	is_unset_target(t_simple_cmds *cmd_list, char *env)
{
	int		j;
	int		len;

	j = 1;
	while (cmd_list->str[j])
	{
		len = ft_strlen(cmd_list->str[j]);
		if (ft_strncmp(env, cmd_list->str[j], len) == 0 && env[len] == '=')
			return (1);
		j++;
	}
	return (0);
}

char	**filter_env_without_vars(t_simple_cmds *cmd_list, char **envp)
{
	int		i;
	int		k;
	char	**new_env;
	int		env_len;

	env_len = 0;
	while (envp && envp[env_len])
		env_len++;
	new_env = malloc(sizeof(char *) * (env_len + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	k = 0;
	while (envp[i])
	{
		if (!is_unset_target(cmd_list, envp[i]))
			new_env[k++] = ft_strdup(envp[i]);
		i++;
	}
	new_env[k] = NULL;
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
