/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aycami <aycami@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:46:39 by aycami            #+#    #+#             */
/*   Updated: 2025/04/21 18:20:28 by aycami           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "../structs.h"
# include <stdio.h>

void	execute(t_simple_cmds *cmd_list);
void	builtin_control(t_simple_cmds *cmd_list);
void	echo_builtin(t_simple_cmds *cmd_list);
char	*path_finder(char *cmd, char **envp);
void	none_built_in(t_simple_cmds *cmd_list);

#endif