/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:02:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/04 11:15:05 by aakyuz           ###   ########.fr       */
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

static void	sort_env_vars(char **sorted_env, int count)
{
	int			i;
	int			j;
	char		*temp;
	size_t	cmp_len;
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			size_t len1 = ft_strlen(sorted_env[j]);
			size_t len2 = ft_strlen(sorted_env[j + 1]);
			cmp_len = len1;
			if (len2 > len1)
				cmp_len = len2;
			if (ft_strncmp(sorted_env[j], sorted_env[j + 1], cmp_len) > 0)
			{
				temp = sorted_env[j];
				sorted_env[j] = sorted_env[j + 1];
				sorted_env[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

static void	print_sorted_env(char **sorted_env)
{
	int		i;
	char	*eq;

	i = 0;
	while (sorted_env[i])
	{
		eq = ft_strchr(sorted_env[i], '=');
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

void	empty_export(char ***envp)
{
	int		count;
	int		i;
	char	**sorted_env;

	count = 0;
	while ((*envp)[count])
		count++;
	sorted_env = malloc(sizeof(char *) * (count + 1));
	if (!sorted_env)
		return ;
	i = 0;
	while (i < count)
	{
		sorted_env[i] = ft_strdup((*envp)[i]);
		i++;
	}
	sorted_env[i] = NULL;
	sort_env_vars(sorted_env, count);
	print_sorted_env(sorted_env);
}

char	*get_env_key(const char *env_str)
{
	int	i;

	i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	return (ft_substr(env_str, 0, i));
}

int	find_env_index(char **envp, const char *key)
{
	int	i;
	int	key_len;

	i = 0;
	key_len = strlen(key);
	while (envp[i])
	{
		if (strncmp(envp[i], key, key_len) == 0 && envp[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

int	is_valid_identifier(const char *s)
{
	int	i;

	i = 0;
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

static void	handle_export_entry(char ***envp, char *entry, int *invalid_found)
{
	char	*key;
	int		idx;
	int		len;
	char	**new_env;
	int		i;

	if (!is_valid_identifier(entry))
	{
		write(2, "minishell: export: `", 21);
		write(2, entry, ft_strlen(entry));
		write(2, "': not a valid identifier\n", 27);
		*invalid_found = 1;
		return ;
	}
	key = get_env_key(entry);
	if (!key)
		exit(1);
	idx = find_env_index(*envp, key);
	if (idx != -1)
	{
		free((*envp)[idx]);
		(*envp)[idx] = ft_strdup(entry);
	}
	else
	{
		len = 0;
		while ((*envp)[len])
			len++;
		new_env = malloc(sizeof(char *) * (len + 2));
		if (!new_env)
			exit(1);
		i = 0;
		while (i < len)
		{
			new_env[i] = ft_strdup((*envp)[i]);
			i++;
		}
		new_env[len] = ft_strdup(entry);
		new_env[len + 1] = NULL;
		free_env(*envp);
		*envp = new_env;
	}
	free(key);
}

void	export_builtin(t_simple_cmds *cmd_list, char ***envp)
{
	int	j;
	int	invalid_found;

	j = 1;
	invalid_found = 0;
	if (cmd_list->flag)
	{
		write(2, "This command only works without the flag.\n", 43);
		cmd_list->return_value = 1;
		return ;
	}
	if (!cmd_list->str[1])
	{
		empty_export(envp);
		cmd_list->return_value = 0;
		return ;
	}
	while (cmd_list->str[j])
	{
		handle_export_entry(envp, cmd_list->str[j], &invalid_found);
		j++;
	}
	if (invalid_found != 0)
		cmd_list->return_value = 1;
	else
		cmd_list->return_value = 0;
}
