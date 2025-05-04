/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:02:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/05 01:13:01 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**new_env_maker(char ***envp, int extra)
{
	char	**new;
	int		len;
	int		i;

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

char	*get_env_key(const char *env_str)
{
	int	i;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	return ft_substr(env_str, 0, i);
}

int find_env_index(char **envp, const char *key)
{
	int	i;
	int	key_len ;

	i = 0;
	key_len = strlen(key);
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
	int	i;
	
	i = 0;
	if (!s || !*s)
		return (0);

	if (s[0] == '=')
		return (0);

	if (!(ft_isalpha(s[0]) || s[0] == '_'))
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
	int		i;
	int		j;
	char	*key;
	int		idx;
	char	*new_entry;
	int		invalid_found;

	j = 1;
	i = 0;
	invalid_found = 0;
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
				write(2, "minishell: export: `", 20);
				write(2, cmd_list->str[j], ft_strlen(cmd_list->str[j]));
				write(2, "': not a valid identifier\n", 26);
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
				while (i < len)
				{
					new_env[i] = ft_strdup((*envp)[i]);
					i++;
				}
				new_env[len] = new_entry;
				new_env[len + 1] = NULL;
			
				free_env(*envp);
				*envp = new_env;
			}
			free(key);
			j++;
		}
	}
	if (invalid_found)
		cmd_list->return_value = 1;
	else
		cmd_list->return_value = 0;
}
