/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyuz <akyuz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:12:25 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/24 23:29:28 by akyuz            ###   ########.fr       */
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
	int		i;
	char	*result;
	int		in_single_quote;
	int		in_double_quote;
	char	*env_var;
	int		j;
	char	*env_value;
	char	*temp;
	char	*new_result;
	char	*final_result;

	if (!str || !*str)
		return (ft_strdup(""));
	result = ft_strdup(str);
	i = 0;
	in_single_quote = 0;
	in_double_quote = 0;
	while (result[i])
	{
		if (result[i] == '\'' && !in_double_quote)
			in_single_quote = !in_single_quote;
		else if (result[i] == '\"' && !in_single_quote)
			in_double_quote = !in_double_quote;
		else if (result[i] == '$' && !in_single_quote)
		{
			env_var = ft_strdup(&result[i + 1]);
			j = 0;
			while (env_var[j] && env_var[j] != ' ' && env_var[j] != '\"'
				&& env_var[j] != '\'' && env_var[j] != '$')
				j++;
			env_var[j] = '\0';
			env_value = NULL;
			if (is_in_vars(env_var, vars))
				env_value = get_var(env_var, vars);
			else
				env_value = getenv(env_var);
			if (env_value)
			{
				temp = ft_strdup(result);
				temp[i] = '\0';
				new_result = ft_strjoin(temp, env_value);
				final_result = ft_strjoin(new_result, &result[i + j + 1]);
				free(temp);
				free(new_result);
				free(result);
				result = final_result;
				i += ft_strlen(env_value) - 1;
			}
			free(env_var);
		}
		i++;
	}
	free(str);
	return (result);
}
