/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:04:39 by ahmtemel          #+#    #+#             */
/*   Updated: 2025/05/04 14:50:20 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	remove_quotes_from_arg(char *str)
{
	if (!str)
		return (0);
	if (str[0] == '\0')
		return (0);
	if ((str[0] == '"' && str[ft_strlen(str) - 1] == '"') || (str[0] == '\''
			&& str[ft_strlen(str) - 1] == '\''))
		return (1);
	return (0);
}

char	*extract_number(char *str)
{
	char	*result;
	int		len;
	int		is_quoted;

	is_quoted = remove_quotes_from_arg(str);
	if (is_quoted)
	{
		len = ft_strlen(str) - 2;
		result = ft_substr(str, 1, len);
	}
	else
	{
		result = ft_strdup(str);
	}
	return (result);
}

static int	get_exit_status(t_simple_cmds *cmd_list, int *flag)
{
	char	*exit_arg;
	char	*num_str;
	int		status;

	exit_arg = cmd_list->content[1];
	num_str = extract_number(exit_arg);
	if (ft_isnum(num_str))
		status = ft_new_atoi(num_str, flag);
	else
		status = 400;
	free(num_str);
	return (status);
}

void	exit_builtin(t_simple_cmds *cmd_list, t_free *free_struct)
{
	int	status;
	int	flag;

	flag = 0;
	if (!(cmd_list->prev) && !(cmd_list->next))
	{
		if (cmd_list->content[1] && cmd_list->content[2])
		{
			perror("minishell: exit: too many arguments\n");
			cmd_list->return_value = 1;
			return ;
		}
		if (cmd_list->content[1])
			status = get_exit_status(cmd_list, &flag);
		else
			status = 0;
		write(STDOUT_FILENO, "exit\n", 5);
		free_all(free_struct);
		exit_builtin_value(status, flag, cmd_list);
	}
}
