/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:25:00 by aycami            #+#    #+#             */
/*   Updated: 2025/05/11 14:20:14 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_getenv_home(char **envp)
{
	int		i;
	char	*prefix;

	i = 0;
	prefix = "HOME=";
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], prefix, 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

void	cd_none_path(t_simple_cmds *cmd_list, char	**path)
{
	write(2, "minishell: cd: ", 15);
	write(2, *path, ft_strlen(*path));
	write(2, ": ", 2);
	write(2, strerror(errno), ft_strlen(strerror(errno)));
	write(2, "\n", 1);
	cmd_list->return_value = 1;
}

void	cd_not_home(t_simple_cmds *cmd_list)
{
	write(2, "minishell: cd: HOME not set\n", 28);
	cmd_list->return_value = 1;
}

void	cd_builtin(t_simple_cmds *cmd_list, char **envp)
{
	char	*path;

	if (!cmd_list->str[1])
	{
		path = ft_getenv_home(envp);
		if (!path)
		{
			cd_not_home(cmd_list);
			return ;
		}
	}
	else if (cmd_list->content[2])
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		cmd_list->return_value = 1;
		return ;
	}
	else
		path = cmd_list->str[1];
	if (chdir(path) != 0)
	{
		cd_none_path(cmd_list, &path);
		return ;
	}
	cmd_list->return_value = 0;
}
