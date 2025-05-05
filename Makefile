# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/17 18:27:43 by aakyuz            #+#    #+#              #
#    Updated: 2025/05/05 09:20:21 by aakyuz           ###   ########.fr        #
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
		parser/free.c                       \
		parser/handle.c                     \
		parser/heredoc.c                    \
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
		main.c                              \
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

a: re
	@rm -f $(OBJ)
	$(MAKE) clean
	clear

.PHONY: all clean fclean re a
