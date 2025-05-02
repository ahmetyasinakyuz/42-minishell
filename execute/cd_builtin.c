/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:25:00 by aycami            #+#    #+#             */
/*   Updated: 2025/05/02 15:52:28 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../minishell.h"

void	cd_builtin(t_simple_cmds *cmd_list)
{
	if (!cmd_list->str[1])
	{
		fprintf(stderr, "minishell: cd: path required\n");
		return;
	}

	if (chdir(cmd_list->str[1]) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", cmd_list->str[1], strerror(errno));
		return;
	}
}