/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:25:00 by aycami            #+#    #+#             */
/*   Updated: 2025/05/02 15:36:47 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include	"../minishell.h"

void	cd_builtin(t_simple_cmds *cmd_list)
{
	// args[0] = "cd", args[1] = path
	if (!cmd_list->str[1])
	{
		printf("argüman yok");
		fprintf(stderr, "minishell: cd: path required\n");
		return;
	}

	if (chdir(cmd_list->str[1]) != 0)
	{
		printf("chdir olmadııı");
		// Hata olduysa perror ile hata mesajı yazdır
		fprintf(stderr, "minishell: cd: %s: %s\n", cmd_list->str[1], strerror(errno));
		return;
	}
}