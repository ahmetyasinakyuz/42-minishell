/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:02:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/04 18:40:02 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**new_env_maker(char ***envp, int extra)
{
	char **new;
	int len;
	int i;

	if (extra <= 0)
		return (NULL);
	len = 0;
	while (envp && (*envp)[len])
		len++;
	new = malloc(sizeof(char *) * (len + extra + 1));
	if (!new)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new[i] = ft_strdup((*envp)[i]);
		i++;
	}
	while (i < len + extra)
	{
		new[i] = NULL;
		i++;
	}
	new[i] = NULL;
	return (new);
}

void	empty_export(char ***envp)
{
	int		i, j;
	char	**sorted_env;
	char	*temp;
	int		count = 0;

	while ((*envp)[count])
		count++;

	sorted_env = malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
		return;

	i = 0;
	while (i < count)
	{
		sorted_env[i] = ft_strdup((*envp)[i]);
		i++;
	}
	sorted_env[i] = NULL;

	for (i = 0; i < count - 1; i++)
	{
		for (j = 0; j < count - i - 1; j++)
		{
			if (ft_strncmp(sorted_env[j], sorted_env[j + 1],
				ft_strlen(sorted_env[j]) > ft_strlen(sorted_env[j + 1]) ?
				ft_strlen(sorted_env[j]) : ft_strlen(sorted_env[j + 1])) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
		}
	}

	i = 0;
	while (sorted_env[i])
	{
		char *eq = ft_strchr(sorted_env[i], '=');
		if (eq)
		{
			int key_len = eq - sorted_env[i];
			write(1, "declare -x ", 11);
			write(1, sorted_env[i], key_len);
			write(1, "=\"", 2);
			write(1, eq + 1, ft_strlen(eq + 1));
			write(1, "\"\n", 2);
		}
		else
		{
			printf("declare -x %s\n", sorted_env[i]);
		}
		free(sorted_env[i]);
		i++;
	}
	free(sorted_env);
}

char *get_env_key(const char *env_str)
{
	int i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	return ft_substr(env_str, 0, i);
}

int find_env_index(char **envp, const char *key)
{
	int i = 0;
	int key_len = strlen(key);
	while (envp[i])
	{
		if (strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return i;
		i++;
	}
	return -1;
}

int	is_valid_identifier(const char *s)
{
	int i = 0;

	if (!s || !(ft_isalpha(s[0]) || s[0] == '_'))
		return (0);
	while (s[i] && s[i] != '=')
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	export_builtin(t_simple_cmds *cmd_list, char ***envp, int flag)
{
	int j = 1;
	char *key;
	int idx;
	char *new_entry;
	int invalid_found = 0;

	if (cmd_list->flag)
	{
		write(2, "This command only works without the flag.\n", 43);
		cmd_list->return_value = 1;
		return;
	}
	if (flag == 0 || !cmd_list->str[1])
	{
		empty_export(envp);
		cmd_list->return_value = 0;
		return;
	}
	if(!(cmd_list->prev) && !(cmd_list->next))
	{
		while (cmd_list->str[j])
		{
			if (!is_valid_identifier(cmd_list->str[j]))
			{
				write(2, "minishell: export: `", 21);
				write(2, cmd_list->str[j], ft_strlen(cmd_list->str[j]));
				write(2, "': not a valid identifier\n", 27);
				invalid_found = 1;
				j++;
				continue;
			}
			new_entry = ft_strdup(cmd_list->str[j]);
			if (!new_entry)
				exit(1);
			key = get_env_key(new_entry);
			if (!key)
			{
				free(new_entry);
				exit(1);
			}
			idx = find_env_index(*envp, key);
			if (idx != -1)
			{
				free((*envp)[idx]);
				(*envp)[idx] = new_entry;
			}
			else
			{
				int len = 0;
				while ((*envp)[len])
					len++;
			
				char **new_env = malloc(sizeof(char *) * (len + 2));
				if (!new_env)
				{
					free(key);
					free(new_entry);
					exit(1);
				}
				for (int i = 0; i < len; i++)
					new_env[i] = ft_strdup((*envp)[i]);
				new_env[len] = new_entry;
				new_env[len + 1] = NULL;
			
				free_env(*envp);
				*envp = new_env;
			}
			free(key);
			j++;
		}
	}
	cmd_list->return_value = invalid_found ? 1 : 0;
}