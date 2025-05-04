/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:45:55 by codespace         #+#    #+#             */
/*   Updated: 2025/05/04 17:46:48 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "../minishell.h"

int	equal_control(char *envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (envp[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

void	env_builtin(t_simple_cmds *cmd_list, char ***envp)
{
	int	i;

	cmd_list->return_value = 0;
	if (cmd_list->flag)
	{
		printf("This command only works without the flag.\n");
		cmd_list->return_value = 1;
		return;
	}
	i = 0;
	while ((*envp)[i])
	{
		if (equal_control((*envp)[i]))
			printf("%s\n", (*envp)[i]);
		i++;
	}
}