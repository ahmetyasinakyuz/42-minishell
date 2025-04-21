/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:59:20 by aycami            #+#    #+#             */
/*   Updated: 2025/04/21 19:14:11 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	echo_builtin(t_simple_cmds *cmd_list)
{
	int	i;

	i = 0;
	cmd_list->raw_command = cmd_list->raw_command + 5;
    while (cmd_list->raw_command[i] == ' ')
        i++;
}
