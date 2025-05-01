/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_ultis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyuz <akyuz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/01 12:46:12 by akyuz             #+#    #+#             */
/*   Updated: 2025/05/01 13:07:49 by akyuz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_simple_cmds	*create_command(t_lexer *start, t_lexer *end)
{
	t_simple_cmds	*cmd;

	if (init_cmd(&cmd))
		return (NULL);
	cmd = setup_command_flags(cmd, start, end);
	if (!cmd)
		return (NULL);
	cmd = setup_command_str(cmd, start, end);
	if (!cmd)
		return (NULL);
	cmd = setup_command_content(cmd, start, end);
	return (cmd);
}

void	add_command(t_simple_cmds **cmd_list, t_simple_cmds *new_cmd)
{
	t_simple_cmds	*temp;

	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		return ;
	}
	temp = *cmd_list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_cmd;
	new_cmd->prev = temp;
}

int	is_var_assignment(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	if (!ft_isalpha(str[i]) && str[i] != '_')
		return (0);
	i++;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	return (str[i] == '=');
}
