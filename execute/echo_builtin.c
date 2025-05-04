/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:59:20 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 16:44:02 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo_builtin(t_simple_cmds *cmd_list)
{
	int	i;
	int	flag;

	i = 1;
	flag = 0;
	if ((cmd_list->content && cmd_list->content[1])
		&& (cmd_list->content[1][0]) == '-' && (cmd_list->content[1][1] == 'n'))
	{
		flag = 1;
		i = 2;
	}
	if (cmd_list->content)
	{
		while (cmd_list->content[i] && cmd_list->content[i + 1])
		{
			printf("%s ", cmd_list->content[i]);
			i++;
		}
		if (cmd_list->content[i])
			printf("%s", cmd_list->content[i]);
	}
	if (flag == 0)
		printf("\n");
	cmd_list->return_value = 0;
}
