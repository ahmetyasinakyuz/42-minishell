/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/02 15:25:00 by aycami            #+#    #+#             */
/*   Updated: 2025/05/03 19:11:16 by aycami           ###   ########.fr       */
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

void	cd_builtin(t_simple_cmds *cmd_list, char **envp)
{
	char	*path;

	if (!cmd_list->str[1])
	{
		path = ft_getenv_home(envp);
		if (!path)
		{
			fprintf(stderr, "minishell: cd: HOME not set\n");
			cmd_list->return_value = 1;
			return ;
		}
	}
	else
		path = cmd_list->str[1];
	if (chdir(path) != 0)
	{
		fprintf(stderr, "minishell: cd: %s: %s\n", path, strerror(errno));
		cmd_list->return_value = 1;
		return ;
	}
	cmd_list->return_value = 0;
}
