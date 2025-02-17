# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aakyuz <aakyuz@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/17 18:27:43 by aakyuz            #+#    #+#              #
#    Updated: 2025/02/17 18:27:44 by aakyuz           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
C_FLAGS = -Wall -Wextra -Werror

SRCS =	parser/parser_utils.c\
		parser/parser.c\
		parser/lexer.c\
		 main.c \

OBJ =$(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@make -C libft/
	@$(CC) $(C_FLAGS) $(OBJ) ./libft/libft.a -o $(NAME) -lreadline

%.o: %.c
	@$(CC) $(C_FLAGS) -c $< -o $@

fclean: clean
	@make fclean -C libft/
	@rm -rf $(NAME) $(NAME_C)

clean:
	@make clean -C libft/
	@rm -rf $(OBJ) $(OBJ_C)


re: fclean all

a: re
	rm -f $(OBJ)
	$(MAKE) clean
	clear

.PHONY: all clean fclean re a
