/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:32 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/23 12:54:07 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

void	remove_token(t_lexer **list, t_lexer *token)
{
	if (token->prev)
		token->prev->next = token->next;
	else
		*list = token->next;
	if (token->next)
		token->next->prev = token->prev;
	token->next = NULL;
	token->prev = NULL;
}

t_lexer	*copy_token(t_lexer *token)
{
	t_lexer	*new_token;

	new_token = malloc(sizeof(t_lexer));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(token->str);
	new_token->token = token->token;
	new_token->i = token->i;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

int	init_cmd(t_simple_cmds **cmd)
{
	*cmd = malloc(sizeof(t_simple_cmds));
	if (!*cmd)
		return (1);
	(*cmd)->num_redirections = 0;
	(*cmd)->hd_file_name = NULL;
	(*cmd)->redirections = NULL;
	(*cmd)->next = NULL;
	(*cmd)->prev = NULL;
	(*cmd)->pipe = 0;
	(*cmd)->str = NULL;
	(*cmd)->flag = NULL;
	return (0);
}

int	count_words(t_lexer *start, t_lexer *end)
{
	int		word_count;
	t_lexer	*current;

	word_count = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && 
			(!current->prev || !is_redirection(current->prev->token)) &&
			!is_flag(current->str))  // Flag değilse say
			word_count++;
		current = current->next;
	}
	return (word_count);
}

void	fill_words(t_simple_cmds *cmd, t_lexer *start, t_lexer *end)
{
	int		i;
	t_lexer	*current;
	char	*temp;
	int		len;

	i = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && 
			(!current->prev || !is_redirection(current->prev->token)) &&
			!is_flag(current->str))  // Flag değilse ekle
		{
			temp = ft_strdup(current->str);
			len = ft_strlen(temp);
			if (len > 0 && temp[len - 1] == '1' && temp[0] == '\'')
				temp[len - 1] = '\0';
			cmd->str[i++] = temp;
		}
		current = current->next;
	}
	cmd->str[i] = NULL;
}

int	is_flag(char *str)
{
	return (str && str[0] == '-');
}

int	count_flags(t_lexer *start, t_lexer *end)
{
	int		flag_count;
	t_lexer	*current;

	flag_count = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && is_flag(current->str) &&
			(!current->prev || current->prev->token == WORD))
			flag_count++;
		current = current->next;
	}
	return (flag_count);
}

void	fill_flags(t_simple_cmds *cmd, t_lexer *start, t_lexer *end)
{
	int		i;
	t_lexer	*current;

	i = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && is_flag(current->str) &&
			(!current->prev || current->prev->token == WORD))
			cmd->flag[i++] = ft_strdup(current->str);
		current = current->next;
	}
	cmd->flag[i] = NULL;
}
