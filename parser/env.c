/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:12:25 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/20 01:47:46 by aakyuz           ###   ########.fr       */
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

void	add_var(t_vars **vars, char *key, char *value)
{
	t_vars	*new_var;
	t_vars	*temp;

	new_var = (t_vars *)malloc(sizeof(t_vars));
	if (!new_var)
		return ;
	new_var->key = key;
	new_var->value = value;
	new_var->next = NULL;
	new_var->prev = NULL;
	if (!*vars)
	{
		*vars = new_var;
		return ;
	}
	temp = *vars;
	while (temp && temp->next)
		temp = temp->next;
	temp->next = new_var;
	new_var->prev = temp;
}

int	is_declare(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	if (str[i] == '=')
		return (1);
	return (0);
}

void	found_var(char *str, t_vars **vars)
{
	char *key;
	char *value;
	int i;

	if (!is_declare(str))
		return ;
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	key = ft_substr(str, 0, i);
	if (str[i] == '=')
		value = ft_strdup(str + i + 1);
	else
		value = ft_strdup("");

	add_var(vars, key, value);
}
