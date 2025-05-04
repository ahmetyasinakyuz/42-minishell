/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_export_builtin.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 01:11:01 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 01:11:46 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_count(char **envp)
{
	int count = 0;
	while (envp[count])
		count++;
	return count;
}

char	**copy_env(char **envp, int count)
{
	char	**copy;
	int		i = 0;

	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return NULL;
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return copy;
}

void	sort_env(char **env, int count)
{
	int		i, j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (ft_strncmp(env[j], env[j + 1],
				ft_strlen(env[j]) > ft_strlen(env[j + 1]) ?
				ft_strlen(env[j]) : ft_strlen(env[j + 1])) > 0)
			{
				temp = env[j];
				env[j] = env[j + 1];
				env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	print_sorted_env(char **env)
{
	int		i = 0;
	char	*eq;

	while (env[i])
	{
		eq = ft_strchr(env[i], '=');
		if (eq)
		{
			int key_len = eq - env[i];
			write(1, "declare -x ", 11);
			write(1, env[i], key_len);
			write(1, "=\"", 2);
			write(1, eq + 1, ft_strlen(eq + 1));
			write(1, "\"\n", 2);
		}
		else
			printf("declare -x %s\n", env[i]);
		free(env[i]);
		i++;
	}
}

void	empty_export(char ***envp)
{
	int		count;
	char	**sorted_env;

	count = env_count(*envp);
	sorted_env = copy_env(*envp, count);
	if (!sorted_env)
		return;
	sort_env(sorted_env, count);
	print_sorted_env(sorted_env);
	free(sorted_env);
}
