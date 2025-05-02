/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:25:00 by aycami            #+#    #+#             */
/*   Updated: 2025/05/02 18:35:46 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../minishell.h"

void	cd_builtin(t_simple_cmds *cmd_list)
{
	char *path;

	if (!cmd_list->str[1])
	{
		path = getenv("HOME");
		if (!path)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			cmd_list->return_value = 1;
			return;
		}
	}
	else
		path = cmd_list->str[1];
	if (chdir(path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
		cmd_list->return_value = 1;
		return;
	}
	cmd_list->return_value = 0;
}
