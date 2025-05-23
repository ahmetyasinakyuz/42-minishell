/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 14:22:37 by aycami            #+#    #+#             */
/*   Updated: 2025/05/11 15:02:14 by aycami           ###   ########.fr       */
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
	int	j;

	len = ft_strlen(input);
	if (len == 0)
		return (0);
	i = len - 1;
	while (i >= 0 && (input[i] == ' ' || input[i] == '\t'))
		i--;
	if (i < 0)
		return (0);
	if (input[i] != '|')
		return (0);
	if (i > 0)
	{
		j = i - 1;
		while (j >= 0 && (input[j] == ' ' || input[j] == '\t'))
			j--;
		if (j >= 0 && input[j] == '|')
			return (0);
	}
	return (is_quote_closed(input));
}

char	*join_input_and_continuation(char *input, char *continuation)
{
	char	*temp;
	char	*combined;

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

char	*get_continuation(char *input)
{
	char	*continuation;
	int		original_stdin;

	original_stdin = dup(STDIN_FILENO);
	g_received_signal = 0;
	setup_continuation_signals();
	continuation = readline("> ");
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	setup_signals();
	if (!continuation || g_received_signal == SIGINT)
	{
		free(input);
		return (NULL);
	}
	if (ft_strlen(continuation) > 0)
		return (join_input_and_continuation(input, continuation));
	free(continuation);
	return (input);
}
