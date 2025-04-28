/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_control.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/04/23 12:28:31 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void i_handle(t_simple_cmds *cmd_list)
{
	if (cmd_list->input_type == IO_PIPE_IN)
	{

	}
	else if (cmd_list->input_type == IO_FILE_IN)
	{
		if (cmd_list->input_fd <= 0)
		{
			if (access(cmd_list->redirections->next->str, F_OK) != 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd_list->redirections->next->str, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				exit(1);
			}
			cmd_list->input_fd = open(cmd_list->redirections->next->str, O_RDONLY);
			if (cmd_list->input_fd < 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd_list->redirections->next->str, 2);
				ft_putstr_fd(": Permission denied\n", 2);
				exit(1);
			}
		}
		dup2(cmd_list->input_fd, STDIN_FILENO);
	}
	else if (cmd_list->input_type == IO_HEREDOC)
	{

	}
	else if (cmd_list->input_type == IO_APPEND)
	{

	}
}

void o_handle(t_simple_cmds *cmd_list)
{
	if (cmd_list->output_type == IO_PIPE_OUT)
	{

	}
	else if (cmd_list->output_type == IO_FILE_OUT)
	{
		if (cmd_list->output_fd <= 0)
		{
			cmd_list->output_fd = open(cmd_list->redirections->next->str, 
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (cmd_list->output_fd < 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd_list->redirections->next->str, 2);
				ft_putstr_fd(": Permission denied\n", 2);
				exit(1);
			}
		}
		dup2(cmd_list->output_fd, STDOUT_FILENO);
	}
	else if (cmd_list->output_type == IO_APPEND)
	{
		if (cmd_list->output_fd <= 0)
		{
			cmd_list->output_fd = open(cmd_list->redirections->next->str, 
				O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (cmd_list->output_fd < 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(cmd_list->redirections->next->str, 2);
				ft_putstr_fd(": Permission denied\n", 2);
				exit(1);
			}
		}
		dup2(cmd_list->output_fd, STDOUT_FILENO);
	}
    else if (cmd_list->input_type == IO_APPEND)
	{

	}
}

void	io_handle(t_simple_cmds *cmd_list)
{
	i_handle(cmd_list);
	o_handle(cmd_list);
}