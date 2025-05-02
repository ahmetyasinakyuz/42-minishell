/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:25:00 by aycami            #+#    #+#             */
/*   Updated: 2025/05/02 17:07:53 by aycami           ###   ########.fr       */
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
			return;
		}
	}
	else
		path = cmd_list->str[1];
	if (chdir(path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
		return;
	}
}
