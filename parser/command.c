/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: akyuz <akyuz@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 23:34:53 by akyuz             #+#                */
/*   Updated: 2025/04/30 17:05:57 by akyuz            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_simple_cmds	*setup_command_flags(t_simple_cmds *cmd, t_lexer *start,
		t_lexer *end)
{
	int	flag_count;

	handle_redirections(cmd, &start);
	if (cmd->redirections)
		handle_heredoc(cmd, cmd->redirections);
	flag_count = count_flags(start, end);
	if (flag_count > 0)
	{
		cmd->flag = malloc(sizeof(char *) * (flag_count + 1));
		if (!cmd->flag)
		{
			free(cmd);
			return (NULL);
		}
		fill_flags(cmd, start, end);
	}
	return (cmd);
}

int	count_content(t_lexer *start, t_lexer *end)
{
	int		count;
	t_lexer	*current;

	count = 0;
	current = start;
	while (current != end && current)
	{
		if (is_redirection(current->token) && current->next)
		{
			current = current->next;
		}
		else
		{
			count++;
		}
		current = current->next;
	}
	return (count);
}

void	fill_content(t_simple_cmds *cmd, t_lexer *start, t_lexer *end)
{
	int		i;
	t_lexer	*current;
	char	*unquoted;

	i = 0;
	current = start;
	while (current != end && current)
	{
		if (is_redirection(current->token) && current->next)
		{
			current = current->next;
		}
		else
		{
			unquoted = remove_quotes(ft_strdup(current->str));
			cmd->content[i++] = unquoted;
		}
		current = current->next;
	}
	cmd->content[i] = NULL;
}

t_simple_cmds	*setup_command_str(t_simple_cmds *cmd, t_lexer *start,
		t_lexer *end)
{
	int	word_count;

	word_count = count_words(start, end);
	if (word_count == 0)
	{
		free(cmd->flag);
		free(cmd);
		return (NULL);
	}
	cmd->str = malloc(sizeof(char *) * (word_count + 1));
	if (!cmd->str)
	{
		free(cmd->flag);
		free(cmd);
		return (NULL);
	}
	fill_words(cmd, start, end);
	return (cmd);
}

t_simple_cmds	*setup_command_content(t_simple_cmds *cmd, t_lexer *start,
		t_lexer *end)
{
	int	content_count;

	content_count = count_content(start, end);
	if (content_count > 0)
	{
		cmd->content = malloc(sizeof(char *) * (content_count + 1));
		if (!cmd->content)
		{
			free(cmd->str);
			free(cmd->flag);
			free(cmd);
			return (NULL);
		}
		fill_content(cmd, start, end);
	}
	return (cmd);
}
