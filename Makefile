NAME = minishell

CC = cc
C_FLAGS = -Wall -Wextra -Werror

SRCS = parser/parser.c\
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
