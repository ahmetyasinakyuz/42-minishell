/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 01:37:46 by aysesudecam       #+#    #+#             */
/*   Updated: 2025/05/10 09:22:17 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_exit(t_vars **vars, char ***envp)
{
	rl_clear_history();
	clear_vars(vars);
	free_env(*envp);
}

int	is_quote_closed(char *input)
{
	int	i;
	int	in_squote;
	int	in_dquote;

	i = 0;
	in_squote = 0;
	in_dquote = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (input[i] == '\"' && !in_squote)
			in_dquote = !in_dquote;
		i++;
	}
	return (!in_squote && !in_dquote);
}

int	is_input_incomplete(char *input)
{
	int	len;
	int	i;

	len = ft_strlen(input);
	if (len == 0)
		return (0);
	i = 0;
	while (i < len && (input[i] == ' ' || input[i] == '\t'))
		i++;
	if (i < len && input[i] == '|')
		return (0);
	i = len - 1;
	while (i >= 0 && (input[i] == ' ' || input[i] == '\t'))
		i--;
	if (i < 0)
		return (0);
	if (input[i] != '|')
		return (0);
	return (is_quote_closed(input));
}

char	*get_continuation(char *input)
{
	char	*continuation;
	char	*combined;
	char	*temp;

	continuation = readline("> ");
	if (!continuation)
		return (input);
	if (ft_strlen(continuation) > 0)
	{
		temp = ft_strjoin(input, " ");
		if (!temp)
		{
			free(continuation);
			return (input);
		}
		combined = ft_strjoin(temp, continuation);
		free(temp);
		free(input);
		free(continuation);
		if (!combined)
			return (NULL);
		return (combined);
	}
	free(continuation);
	return (input);
}
