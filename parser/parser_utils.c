/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 18:27:32 by aakyuz            #+#    #+#             */
/*   Updated: 2025/05/11 10:53:08 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*process_dollar_sign(char *result, int *i, t_vars **vars)
{
	char	*env_var;
	char	*env_value;

	env_var = ft_strdup("$");
	env_value = get_env_value(env_var, vars);
	free(env_var);
	result = join_env_parts(result, *i, env_value, 1);
	*i = -1;
	return (result);
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
	(*cmd)->content = NULL;
	(*cmd)->input_type = IO_STDIN;
	(*cmd)->output_type = IO_STDOUT;
	(*cmd)->input_fd = 0;
	(*cmd)->output_fd = 1;
	(*cmd)->return_value = 0;
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
		if (current->token == WORD && (!current->prev
				|| !is_redirection(current->prev->token))
			&& !is_flag(current->str))
			word_count++;
		current = current->next;
	}
	return (word_count);
}

void	fill_words(t_simple_cmds *cmd, t_lexer *start, t_lexer *end)
{
	int		i;
	t_lexer	*current;
	char	*unquoted;
	char	*temp;

	i = 0;
	current = start;
	while (current != end && current)
	{
		if (current->token == WORD && (!current->prev
				|| !is_redirection(current->prev->token)))
		{
			temp = ft_strdup(current->str);
			if (!is_flag(temp))
			{
				unquoted = remove_quotes(temp);
				cmd->str[i++] = unquoted;
			}
			else
				free(temp);
		}
		current = current->next;
	}
	cmd->str[i] = NULL;
}

char	*remove_quotes(char *str)
{
	int		i;
	int		j;
	char	*result;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	process_quotes(str, result, &i, &j);
	free(str);
	return (result);
}
