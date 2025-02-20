/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:12:25 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/20 04:21:31 by aakyuz           ###   ########.fr       */
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

	if (!str || !*str)
		return (ft_strdup(""));
	if (*str != '$')
		return (str);
	env_var = ft_strdup(str + 1);
	if (is_in_vars(env_var, vars))
	{
		env = get_var(env_var, vars);
		result = ft_strdup(env);
	}
	else
	{
		env = getenv(env_var);
		if (env)
			result = ft_strdup(env);
		else
			result = ft_strdup("");
	}
	free(env_var);
	free(str);
	return (result);
}
