/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:16:25 by aycami            #+#    #+#             */
/*   Updated: 2025/05/02 15:52:36 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd_builtin(t_simple_cmds *cmd_list)
{
	char *cwd;

	cwd = getcwd(NULL, 0);
	if (cmd_list->flag)
	{
		printf("This command only works without the flag.\n");
		cmd_list->return_value = 1;
		return;
	}
	if (cwd == NULL)
	{
		perror("pwd");
		return;
	}
	printf("%s\n", cwd);
	free(cwd);
}