/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:12:25 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/25 13:02:46 by aakyuz           ###   ########.fr       */
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

	env_value = NULL;
	if (is_in_vars(env_var, vars))
		env_value = get_var(env_var, vars);
	else
		env_value = getenv(env_var);
	return (env_value);
}

static char	*join_env_parts(char *result, int i, char *env_value, int j)
{
	char	*temp;
	char	*final_result;

	temp = ft_strdup(result);
	temp[i] = '\0';
	final_result = ft_strjoin(temp, env_value);
	free(temp);
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

	env_var = ft_strdup(&result[i + 1]);
	j = 0;
	while (env_var[j] && env_var[j] != ' ' && env_var[j] != '\"'
		&& env_var[j] != '\'' && env_var[j] != '$')
		j++;
	env_var[j] = '\0';
	env_value = get_env_value(env_var, vars);
	if (env_value)
		result = join_env_parts(result, i, env_value, j);
	free(env_var);
	return (result);
}
