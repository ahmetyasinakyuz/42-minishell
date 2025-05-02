/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:26:00 by aycami            #+#    #+#             */
/*   Updated: 2025/05/02 18:50:58 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd_builtin(t_simple_cmds *cmd_list)
{
	char *cwd;

	// Initialize return value before any operations
	cmd_list->return_value = 0;
	
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		printf("%s\n", cwd);
		free(cwd);
	}
	else
	{
		perror("getcwd");
		cmd_list->return_value = 1;
	}
}
