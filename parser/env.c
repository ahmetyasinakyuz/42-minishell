/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:12:25 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/23 14:43:40 by aakyuz           ###   ########.fr       */
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
	int		in_single_quote;
	char	*result;

	if (!str || !*str)
		return (ft_strdup(""));
	result = ft_strdup(str);
	i = 0;
	in_single_quote = 0;
	while (result[i])
	{
		if (result[i] == '\'')
			in_single_quote = !in_single_quote;
		else if (result[i] == '$' && !in_single_quote)
		{
			char *env_var = ft_strdup(&result[i + 1]);
			int j = 0;
			while (env_var[j] && env_var[j] != ' ' && env_var[j] != '\"' && env_var[j] != '\'')
				j++;
			env_var[j] = '\0';

			char *env_value = NULL;
			if (is_in_vars(env_var, vars))
				env_value = get_var(env_var, vars);
			else
				env_value = getenv(env_var);

			if (env_value)
			{
				char *temp = ft_strdup(result);
				temp[i] = '\0';
				char *new_result = ft_strjoin(temp, env_value);
				new_result = ft_strjoin(new_result, &result[i + j + 1]);
				free(temp);
				free(result);
				result = new_result;
				i--; // Yeni eklenen değeri de kontrol etmek için
			}
			free(env_var);
		}
		i++;
	}
	free(str);
	return (result);
}
