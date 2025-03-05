/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 04:21:10 by aakyuz            #+#    #+#             */
/*   Updated: 2025/03/05 18:06:19 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

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

int	is_alraday_declared(t_vars **vars, char *key, char *value)
{
	t_vars	*temp;

	temp = *vars;
	while (temp)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
		{
			free(temp->value);
			temp->value = value;
			free(key);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

void	found_var(char *str, t_vars **vars)
{
	char	*key;
	char	*value;
	int		i;

	if (!is_declare(str))
		return ;
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	key = ft_substr(str, 0, i);
	value = ft_strdup(str + i + 1);
	if (is_alraday_declared(vars, key, value))
		return ;
	add_var(vars, key, value);
}
