/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_export_builtin.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 01:11:01 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 01:20:09 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_count(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

char	**copy_env(char **envp, int count)
{
	char	**copy;
	int		i;

	i = 0;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	while (i < count)
	{
		copy[i] = ft_strdup(envp[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

void	sort_env(char **env, int count)
{
	int		i[2];
	int		max_len;
	char	*temp;

	i[0] = 0;
	while (i[0] < count - 1)
	{
		i[1] = 0;
		while (i[1] < count - i[0] - 1)
		{
			if (ft_strlen(env[i[1]]) > ft_strlen(env[i[1] + 1]))
				max_len = ft_strlen(env[i[1]]);
			else
				max_len = ft_strlen(env[i[1] + 1]);
			if (ft_strncmp(env[i[1]], env[i[1] + 1], max_len) > 0)
			{
				temp = env[i[1]];
				env[i[1]] = env[i[1] + 1];
				env[i[1] + 1] = temp;
			}
			i[1]++;
		}
		i[0]++;
	}
}

void	print_sorted_env(char **env)
{
	int		i;
	char	*eq;
	int		key_len;

	i = 0;
	while (env[i])
	{
		eq = ft_strchr(env[i], '=');
		if (eq)
		{
			key_len = eq - env[i];
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
		return ;
	sort_env(sorted_env, count);
	print_sorted_env(sorted_env);
	free(sorted_env);
}
