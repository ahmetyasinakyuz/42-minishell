/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 14:48:51 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 14:50:33 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_all(t_free *free_struct)
{
	free_lexer_list(free_struct->token_list);
	free(free_struct->pids);
	clear_vars(free_struct->vars);
	free_env(free_struct->envp);
	rl_clear_history();
}

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

void	exit_builtin_value(int i, int flag, t_simple_cmds *cmd_list)
{
	if (i == 400 || flag == -1)
	{
		write(2, "minishell: exit: ", 17);
		write(2, cmd_list->content[1], ft_strlen(cmd_list->content[1]));
		write(2, ": ", 2);
		write(2, "numeric argument required\n", 26);
		free_command_list(cmd_list);
		exit(2);
	}
	else
	{
		free_command_list(cmd_list);
		exit(i);
	}
}
