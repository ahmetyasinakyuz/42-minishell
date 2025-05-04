/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_two.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 01:36:29 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 01:39:49 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

void	export_with_flag_error(t_simple_cmds *cmd_list)
{
	write(2, "This command only works without the flag.\n", 43);
	cmd_list->return_value = 1;
}

void	print_invalid_identifier(char *str)
{
	write(2, "minishell: export: `", 20);
	write(2, str, ft_strlen(str));
	write(2, "': not a valid identifier\n", 26);
}

void	update_env_entry(char **envp, int idx, char *new_entry)
{
	free(envp[idx]);
	envp[idx] = new_entry;
}
