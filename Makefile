# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/17 18:27:43 by aakyuz            #+#    #+#              #
#    Updated: 2025/04/22 19:00:39 by aakyuz           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
C_FLAGS = -Wall -Wextra -Werror

SRCS =	libs/get_next_line/get_next_line.c  \
		parser/flags.c                      \
		parser/command.c                    \
		parser/token.c                      \
		parser/redirections.c               \
		parser/quotes.c                     \
		parser/parser_utils.c               \
		parser/parser.c                     \
		parser/lexer.c                      \
		parser/lexer_utils.c                \
		parser/env.c                        \
		parser/var.c                        \
		parser/free.c                       \
		parser/handle.c                     \
		parser/heredoc.c                    \
		execute/builtin_control.c           \
		execute/none_built_in.c             \
		execute/execute.c                   \
		execute/path_finder.c               \
		execute/echo_builtin.c              \
		execute/io_handle.c                 \
		main.c                              \
		signal_handler.c                    \

OBJ =$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libs/libft/
	@make -C libs/ft_printf/
	@$(CC) $(C_FLAGS) $(OBJ) ./libs/libft/libft.a ./libs/ft_printf/libftprintf.a -o $(NAME) -lreadline

%.o: %.c
	@$(CC) $(C_FLAGS) -c $< -o $@

fclean: clean
	@make fclean -C libs/libft/
	@make fclean -C libs/ft_printf/
	@rm -rf $(NAME) $(NAME_C)

clean:
	@make clean -C libs/libft/
	@make clean -C libs/ft_printf/
	@rm -rf $(OBJ) $(OBJ_C)


re: fclean all

a: re
	@rm -f $(OBJ)
	$(MAKE) clean
	clear

.PHONY: all clean fclean re a
