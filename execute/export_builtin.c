/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 11:02:14 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/05 01:38:24 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_env_entry(char ***envp, char *new_entry)
{
	int		len;
	int		i;
	char	**new_env;

	len = 0;
	i = 0;
	while ((*envp)[len])
		len++;
	new_env = malloc(sizeof(char *) * (len + 2));
	if (!new_env)
	{
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

void	process_export_entry(char *arg, char ***envp)
{
	char	*key;
	char	*new_entry;
	int		idx;

	new_entry = ft_strdup(arg);
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
		update_env_entry(*envp, idx, new_entry);
	else
		add_env_entry(envp, new_entry);
	free(key);
}

void	handle_export_arguments(t_simple_cmds *cmd_list, char ***envp)
{
	int	j;
	int	invalid_found;

	j = 1;
	invalid_found = 0;
	while (cmd_list->str[j])
	{
		if (!is_valid_identifier(cmd_list->str[j]))
		{
			print_invalid_identifier(cmd_list->str[j]);
			invalid_found = 1;
			j++;
			continue ;
		}
		process_export_entry(cmd_list->str[j], envp);
		j++;
	}
	if (invalid_found)
		cmd_list->return_value = 1;
	else
		cmd_list->return_value = 0;
}

void	export_builtin(t_simple_cmds *cmd_list, char ***envp, int flag)
{
	if (cmd_list->flag)
		return (export_with_flag_error(cmd_list));
	if (flag == 0 || !cmd_list->str[1])
	{
		empty_export(envp);
		cmd_list->return_value = 0;
		return ;
	}
	if (!(cmd_list->prev) && !(cmd_list->next))
		handle_export_arguments(cmd_list, envp);
}
