/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 12:18:56 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	builtin_control(t_simple_cmds *cmd_list, t_free *free_struct)
{
	if (!cmd_list->str || !cmd_list->str[0])
		exit(0);
	else if (ft_strncmp("echo", cmd_list->str[0], 5) == 0)
		echo_builtin(cmd_list);
	else if (ft_strncmp("pwd", cmd_list->str[0], 4) == 0)
		pwd_builtin(cmd_list);
	else if (ft_strncmp("env", cmd_list->str[0], 4) == 0)
		env_builtin(cmd_list, free_struct->envp);
	else
		none_built_in(cmd_list, free_struct->envp);

	exit(cmd_list->return_value);
}
