/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_handle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:27:12 by aycami            #+#    #+#             */
/*   Updated: 2025/05/04 17:15:31 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"

/* Remove all quotes from filename for proper handling of adjacent quoted strings */
char	*unquote_filename(char *str)
{
	char	*result;
	int		i;
	int		j;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	result = malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	while (i < len)
	{
		if (str[i] != '"' && str[i] != '\'')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	return (result);
}

void	i_handle(t_simple_cmds *cmd_list)
{
	t_lexer	*current;
	char	*unquoted_filename;

	if (cmd_list->input_type == IO_PIPE_IN)
	{
		dup2(cmd_list->input_fd, STDIN_FILENO);
	}
	else if (cmd_list->input_type == IO_HEREDOC)
	{
		if (cmd_list->hd_file_name)
		{
			cmd_list->input_fd = open(cmd_list->hd_file_name, O_RDONLY);
			if (cmd_list->input_fd < 0)
			{
				ft_putstr_fd("minishell: heredoc: Cannot open temporary file\n",
					2);
				exit(1);
			}
			dup2(cmd_list->input_fd, STDIN_FILENO);
		}
	}
	current = cmd_list->redirections;
	while (current && current->next)
	{
		if (current->token == REDIRECT_IN)
		{
			unquoted_filename = unquote_filename(current->next->str);
			if (access(unquoted_filename, F_OK) != 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(unquoted_filename, 2);
				ft_putstr_fd(": No such file or directory\n", 2);
				free(unquoted_filename);
				exit(1);
			}
			cmd_list->input_fd = open(unquoted_filename, O_RDONLY);
			if (cmd_list->input_fd < 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(unquoted_filename, 2);
				ft_putstr_fd(": Permission denied\n", 2);
				free(unquoted_filename);
				exit(1);
			}
			free(unquoted_filename);
			dup2(cmd_list->input_fd, STDIN_FILENO);
		}
		if (current->next->next)
			current = current->next->next;
		else
			break ;
	}
}

void	o_handle(t_simple_cmds *cmd_list)
{
	t_lexer	*current;
	char	*unquoted_filename;

	if (cmd_list->output_type == IO_PIPE_OUT)
	{
		dup2(cmd_list->output_fd, STDOUT_FILENO);
	}
	current = cmd_list->redirections;
	while (current && current->next)
	{
		unquoted_filename = NULL;
		if (current->token == REDIRECT_OUT)
		{
			unquoted_filename = unquote_filename(current->next->str);
			cmd_list->output_fd = open(unquoted_filename,
					O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (cmd_list->output_fd < 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(unquoted_filename, 2);
				ft_putstr_fd(": Permission denied\n", 2);
				free(unquoted_filename);
				exit(1);
			}
			free(unquoted_filename);
			dup2(cmd_list->output_fd, STDOUT_FILENO);
		}
		else if (current->token == REDIRECT_APPEND)
		{
			unquoted_filename = unquote_filename(current->next->str);
			cmd_list->output_fd = open(unquoted_filename,
					O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (cmd_list->output_fd < 0)
			{
				ft_putstr_fd("minishell: ", 2);
				ft_putstr_fd(unquoted_filename, 2);
				ft_putstr_fd(": Permission denied\n", 2);
				free(unquoted_filename);
				exit(1);
			}
			free(unquoted_filename);
			dup2(cmd_list->output_fd, STDOUT_FILENO);
		}
		if (current->next->next)
			current = current->next->next;
		else
			break ;
	}
}

void	io_handle(t_simple_cmds *cmd_list)
{
	o_handle(cmd_list);
	i_handle(cmd_list);
}
