/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_extra.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahmtemel <ahmtemel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/11 13:35:23 by ahmtemel          #+#    #+#             */
/*   Updated: 2025/05/11 13:38:15 by ahmtemel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_maker(char **envp)
{
	int		i;
	char	**local_envp;

	i = 0;
	while (envp[i])
		i++;
	local_envp = malloc(sizeof(char *) * (i + 1));
	if (!local_envp)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		local_envp[i] = ft_strdup(envp[i]);
		i++;
	}
	local_envp[i] = NULL;
	return (local_envp);
}

int	main(int argc, char **argv, char **envp)
{
	t_vars	*vars;
	char	**local_envp;

	local_envp = env_maker(envp);
	vars = NULL;
	(void)argv;
	if (argc == 1)
		run_shell(&vars, &local_envp);
	else
		printf("Usage: ./minishell\n");
	return (0);
}
