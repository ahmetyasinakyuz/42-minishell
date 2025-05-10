/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_ultis.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 09:14:48 by aycami            #+#    #+#             */
/*   Updated: 2025/05/10 12:21:53 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

char	*get_env_value(char *env_var, t_vars **vars)
{
	char	*env_value;
	pid_t	shell_pid;

	if (ft_strncmp(env_var, "$", 2) == 0)
	{
		shell_pid = get_shell_pid();
		if (shell_pid > 0)
			return (pid_to_string(shell_pid));
		return (ft_strdup("0"));
	}
	if (ft_strncmp(env_var, "?", 2) == 0)
	{
		if (is_in_vars(env_var, vars))
		{
			env_value = get_var(env_var, vars);
			if (env_value && *env_value)
				return (ft_strdup(env_value));
		}
		return (ft_strdup("0"));
	}
	env_value = NULL;
	if (is_in_vars(env_var, vars))
		env_value = get_var(env_var, vars);
	else
		env_value = getenv(env_var);
	if (!env_value || !*env_value)
		return (ft_strdup(""));
	return (ft_strdup(env_value));
}
