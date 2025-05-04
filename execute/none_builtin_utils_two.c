/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   none_builtin_utils_two.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 02:25:53 by aycami            #+#    #+#             */
/*   Updated: 2025/05/05 02:26:37 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_cmd_not_found(char *cmd, int *ret)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd, 2);
	ft_putstr_fd(": command not found\n", 2);
	*ret = 127;
}

void	handle_command_error(t_simple_cmds *cmd_list, char *cmd, char *path)
{
	if (access(path, F_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		cmd_list->return_value = 127;
		return ;
	}
	else if (is_directory(path))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		cmd_list->return_value = 126;
		return ;
	}
	else if (access(path, X_OK) != 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		cmd_list->return_value = 126;
		return ;
	}
}

int	is_invalid_command(t_simple_cmds *cmd_list)
{
	if (!cmd_list->str[0])
		return (1);
	if (ft_strchr(cmd_list->str[0], '='))
		return (1);
	if (cmd_list->str[0][0] == '\0')
		return (1);
	return (0);
}
