#ifndef PARSER_H
# define PARSER_H

# include "../libft/libft.h"
# include <stdio.h>

typedef enum e_tokens
{
	WORD,
	PIPE,
	REDIRECT_IN,
	REDIRECT_OUT,
	QUOTE,
	DQUOTE
}					t_tokens;

void				parser(char *input);
int					check_quotes(char *input);
void				lexer(char *input);

typedef struct s_lexer
{
	char			*str;
	t_tokens		token;
	int				i;
	struct s_lexer	*next;
	struct s_lexer	*prev;
}					t_lexer;

t_lexer				*create_token(char *str, t_tokens token_type, int index);
void				add_token(t_lexer **list, t_lexer *new_token);
t_tokens			check_token_type(char *str);
void				free_lexer_list(t_lexer *list);

#endif
