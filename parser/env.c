/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:12:25 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/22 18:19:22 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

int	is_in_vars(char *key, t_vars **vars)
{
	t_vars	*temp;

	if (!vars || !*vars)
		return (0);
	temp = *vars;
	while (temp)
	{
		if (temp->key && ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
			return (1);
		temp = temp->next;
	}
	return (0);
}

char	*get_var(char *key, t_vars **vars)
{
	t_vars	*temp;

	if (!vars || !*vars)
		return (NULL);
	temp = *vars;
	while (temp)
	{
		if (temp->key && ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
			return (temp->value);
		temp = temp->next;
	}
	return (NULL);
}

char	*is_dolar(char *str, t_vars **vars)
{
	char	*env;
	char	*env_var;
	char	*result;
	char	*temp;
	int		i;
	int		j;

	if (!str || !*str)
		return (ft_strdup(""));
	temp = ft_strdup(str);
	i = 0;
	while (temp[i])
	{
		if (temp[i] == '$')
		{
			env_var = ft_strdup(&temp[i + 1]);
			j = 0;
			while (env_var[j] && env_var[j] != ' ')
				j++;
			env_var[j] = '\0';
			if (is_in_vars(env_var, vars))
				env = get_var(env_var, vars);
			else
				env = getenv(env_var);
			if (env)
			{
				result = ft_strdup(temp);
				result[i] = '\0';
				result = ft_strjoin(result, env);
				result = ft_strjoin(result, &temp[i + j + 1]);
				free(temp);
				temp = result;
			}
			free(env_var);
		}
		i++;
	}
	free(str);
	return (temp);
}
