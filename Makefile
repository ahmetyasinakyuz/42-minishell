# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aycami <aycami@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/17 18:27:43 by aakyuz            #+#    #+#              #
#    Updated: 2025/05/11 14:22:48 by aycami           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
C_FLAGS = -Wall -Wextra -Werror

SRCS =	parser/flags.c                      \
		parser/command.c                    \
		parser/command_ultis.c              \
		parser/token.c                      \
		parser/token_ultis.c                \
		parser/redirections.c               \
		parser/quotes.c                     \
		parser/parser_utils.c               \
		parser/parser.c                     \
		parser/lexer.c                      \
		parser/lexer_utils.c                \
		parser/env.c                        \
		parser/env_ultis.c                  \
		parser/pid_utils.c                  \
		parser/free.c                       \
		parser/handle.c                     \
		parser/heredoc.c                    \
		parser/heredoc_ultis.c              \
		parser/herecoc_ultis2.c             \
		parser/var.c                        \
		parser/pid.c                        \
		execute/builtin_control.c           \
		execute/none_built_in.c             \
		execute/none_builtin_utils_one.c    \
		execute/none_builtin_utils_two.c    \
		execute/execute.c                   \
		execute/path_finder.c               \
		execute/echo_builtin.c              \
		execute/env_builtin.c               \
		execute/export_builtin.c            \
		execute/empty_export_builtin.c      \
		execute/export_utils_one.c          \
		execute/export_utils_two.c          \
		execute/unset_builtin.c             \
		execute/pwd_builtin.c               \
		execute/cd_builtin.c                \
		execute/io_handle.c                 \
		execute/i_handle.c                  \
		execute/o_handle.c                  \
		execute/ft_new_atoi.c               \
		signal_handler.c                    \
		signal_handler_utils.c              \
		main.c                              \
		main_utils.c                        \
		main_extra.c                        \
		execute/execute_utils_oth.c	        \
		execute/execute_utils.c

OBJ =$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/
	@$(CC) $(C_FLAGS) $(OBJ) ./libft/libft.a -o $(NAME) -lreadline

%.o: %.c
	@$(CC) $(C_FLAGS) -c $< -o $@

fclean: clean
	@make fclean -C libft
	@rm -rf $(NAME) $(NAME_C)

clean:
	@make clean -C libft
	@rm -rf $(OBJ) $(OBJ_C)


re: fclean all

.PHONY: all clean fclean re
