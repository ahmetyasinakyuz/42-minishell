/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:46:39 by aycami            #+#    #+#             */
/*   Updated: 2025/04/21 15:36:47 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "../minishell.h"

void builtin_control(t_simple_cmds *cmd_list,char **env);
char	*path_finder(char *cmd, char **envp);


#endif 