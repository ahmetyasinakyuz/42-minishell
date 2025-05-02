/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 14:40:27 by codespace         #+#    #+#             */
/*   Updated: 2025/05/02 18:54:02 by aakyuz           ###   ########.fr       */
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
	int i;

	i = 0;
	while((*envp)[i])
	{
		printf("declare -x %s\n", (*envp)[i]);
		i++;
	}
}
#include <string.h> // strcmp, strncmp

char *get_env_key(const char *env_str)
{
	int i = 0;
	while (env_str[i] && env_str[i] != '=')
		i++;
	return ft_substr(env_str, 0, i); // malloc'lı substring fonksiyonu
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
void	export_builtin(t_simple_cmds *cmd_list, char ***envp)
{
	int j = 1;
	char *key;
	int idx;
	char *new_entry;

	if (cmd_list->flag)
	{
		printf("This command only works without the flag.\n");
		cmd_list->return_value = 1;
		return;
	}
	if (!cmd_list->str[1])
	{
		empty_export(envp);
		cmd_list->return_value = 0;
		return;
	}

	while (cmd_list->str[j])
	{
		new_entry = ft_strdup(cmd_list->str[j]); // örn: ASA=MASA
		if (!new_entry)
			exit(1);

		key = get_env_key(new_entry); // ASA
		if (!key)
		{
			free(new_entry);
			exit(1);
		}

		idx = find_env_index(*envp, key); // ASA varsa bul
		if (idx != -1)
		{
			// Varsa, eski değeri sil ve yenisini koy
			free((*envp)[idx]);
			(*envp)[idx] = new_entry;
		}
		else
		{
			// Yoksa yeni bir alan aç ve ekle
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
				new_env[i] = (*envp)[i];
			new_env[len] = new_entry;
			new_env[len + 1] = NULL;
			free(*envp);
			*envp = new_env;
		}
		free(key);
		j++;
	}
	cmd_list->return_value = 0;
}
