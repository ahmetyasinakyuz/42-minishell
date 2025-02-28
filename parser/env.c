/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyuz <akyuz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:12:25 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/28 11:20:23 by akyuz            ###   ########.fr       */
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

static char	*get_env_value(char *env_var, t_vars **vars)
{
	char	*env_value;
	char	*result;

	if (is_in_vars(env_var, vars))
	{
		env_value = get_var(env_var, vars);
		if (!env_value || !*env_value)
			return (ft_strdup(""));
		result = ft_strdup(env_value);
		return (result);
	}
	env_value = getenv(env_var);
	if (!env_value)
		return (ft_strdup(""));
	result = ft_strdup(env_value);
	return (result);
}

static char	*join_env_parts(char *result, int i, char *env_value, int j)
{
	char	*temp;
	char	*final_result;

	temp = ft_substr(result, 0, i);
	if (!temp)
	{
		free(env_value);
		free(result);
		return (NULL);
	}
	final_result = ft_strjoin(temp, env_value);
	free(temp);
	free(env_value);
	if (!final_result)
	{
		free(result);
		return (NULL);
	}
	temp = ft_strjoin(final_result, &result[i + j + 1]);
	free(final_result);
	free(result);
	return (temp);
}

char	*replace_env_var(char *result, int i, t_vars **vars)
{
	char	*env_var;
	int		j;
	char	*env_value;

	j = 1;
	while (result[i + j] && (ft_isalnum(result[i + j]) || result[i + j] == '_'))
		j++;
	env_var = ft_substr(result, i + 1, j - 1);
	env_value = get_env_value(env_var, vars);
	free(env_var);
	result = join_env_parts(result, i, env_value, j - 1);
	return (result);
}
