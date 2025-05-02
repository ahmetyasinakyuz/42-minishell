/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 14:45:55 by codespace         #+#    #+#             */
/*   Updated: 2025/05/02 16:56:30 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void env_builtin(t_simple_cmds *cmd_list, char ***envp)
{
	int	i;

	if(cmd_list->flag)
	{
		printf("This command only works without the flag.");
		cmd_list->return_value = 1;
		exit(0);
	}

	i = 0;
	while ((*envp)[i])
	{
		if()
		{
			printf("%s\n", (*envp)[i]);
			i++;
		}
	}
}
