/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:12:25 by aakyuz            #+#    #+#             */
/*   Updated: 2025/02/19 23:36:07 by aakyuz           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"

char	*is_dolar(char *str)
{
	char	*env;
	char	*env_var;
	char	*result;

	if (!str || !*str)
		return (ft_strdup(""));
	if (*str != '$')
		return (str);
	env_var = ft_strdup(str + 1);
	env = getenv(env_var);
	free(env_var);
	if (!env)
		result = ft_strdup("");
	else
		result = ft_strdup(env);
	free(str);
	return (result);
}
