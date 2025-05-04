/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmtemel <ahmtemel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 13:04:39 by ahmtemel          #+#    #+#             */
/*   Updated: 2025/05/04 13:04:40 by ahmtemel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_all(t_free *free_struct)
{
	free_lexer_list(free_struct->token_list);
	free(free_struct->pids);
	clear_vars(free_struct->vars);
	free_env(free_struct->envp);
	rl_clear_history();
}

void	exit_builtin_value(int i, int flag, t_simple_cmds *cmd_list)
{
	if (i == 400 || flag == -1)
	{
		write(2, "minishell: exit: ", 17);
		write(2, cmd_list->content[1], ft_strlen(cmd_list->content[1]));
		write(2, ": ", 2);
		write(2, "numeric argument required\n", 26);
		free_command_list(cmd_list);
		exit(2);
	}
	else
	{
		free_command_list(cmd_list);
		exit(i);
	}
}

int	ft_isnum(char *str)
{
	int	i;

	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	remove_quotes_from_arg(char *str)
{
	if (!str)
		return (0);
	
	if (str[0] == '\0')
		return (0);
		
	if ((str[0] == '"' && str[ft_strlen(str) - 1] == '"') || 
		(str[0] == '\'' && str[ft_strlen(str) - 1] == '\''))
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

void	exit_builtin(t_simple_cmds *cmd_list, t_free *free_struct)
{
	int		i;
	int		flag;
	char	*exit_arg;
	char	*num_str;

	i = 0;
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
		{
			exit_arg = cmd_list->content[1];
			num_str = extract_number(exit_arg);
			
			if (ft_isnum(num_str))
				i = ft_new_atoi(num_str, &flag);
			else
				i = 400;
				
			free(num_str);
		}
		write(STDOUT_FILENO, "exit\n", 5);
		free_all(free_struct);
		exit_builtin_value(i, flag, cmd_list);
	}
}