/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:59:20 by aycami            #+#    #+#             */
/*   Updated: 2025/04/21 17:46:11 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

void echo_builtin(t_simple_cmds *cmd_list)
{
    int i;

    i = 5;
    while(cmd_list->raw_command[i])
    {
        write(1, &cmd_list->raw_command[i], 1);
    }
}