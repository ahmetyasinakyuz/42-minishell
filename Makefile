# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/17 18:27:43 by aakyuz            #+#    #+#              #
#    Updated: 2025/02/18 16:07:58 by aakyuz           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
C_FLAGS = -Wall -Wextra -Werror

SRCS =	libs/get_next_line/get_next_line.c\
		parser/parser_utils.c\
		parser/parser.c\
		parser/lexer.c\
		main.c \

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
	rm -f $(OBJ)
	$(MAKE) clean
	clear

.PHONY: all clean fclean re a
