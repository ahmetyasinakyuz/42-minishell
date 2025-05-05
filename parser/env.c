/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:12:25 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/05 09:15:04 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*join_env_parts(char *result, int i, char *env_value, int j)
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
	if (result[i + 1] == '?')
	{
		j = 2;
		env_var = ft_strdup("?");
		env_value = get_env_value(env_var, vars);
		free(env_var);
		result = join_env_parts(result, i, env_value, j - 1);
		return (result);
	}
	if (ft_isdigit(result[i + j]))
		j++;
	else
		while (result[i + j] && (ft_isalnum(result[i + j]) || result[i
					+ j] == '_'))
			j++;
	env_var = ft_substr(result, i + 1, j - 1);
	env_value = get_env_value(env_var, vars);
	free(env_var);
	result = join_env_parts(result, i, env_value, j - 1);
	return (result);
}

int	update_existing_var(t_vars **vars, char *key, char *value)
{
	t_vars	*temp;

	if (!vars || !*vars)
		return (0);
	temp = *vars;
	while (temp)
	{
		if (temp->key && ft_strncmp(temp->key, key, ft_strlen(key) + 1) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(value);
			return (1);
		}
		temp = temp->next;
	}
	return (0);
}

t_vars	*create_var_node(char *key, char *value)
{
	t_vars	*new_var;

	new_var = malloc(sizeof(t_vars));
	if (!new_var)
		return (NULL);
	new_var->key = ft_strdup(key);
	new_var->value = ft_strdup(value);
	new_var->next = NULL;
	new_var->prev = NULL;
	return (new_var);
}

void	add_static_var(t_vars **vars, char *key, char *value)
{
	t_vars	*new_var;
	t_vars	*temp;

	if (!vars)
		return ;
	if (update_existing_var(vars, key, value))
		return ;
	new_var = create_var_node(key, value);
	if (!new_var)
		return ;
	if (!*vars)
	{
		*vars = new_var;
		return ;
	}
	temp = *vars;
	while (temp->next)
		temp = temp->next;
	temp->next = new_var;
	new_var->prev = temp;
}
