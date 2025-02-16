#include "parser.h"

t_lexer	*create_token(char *str, t_tokens token_type, int index)
{
	t_lexer	*new_token;

	new_token = (t_lexer *)malloc(sizeof(t_lexer));
	if (!new_token)
		return (NULL);
	new_token->str = ft_strdup(str);
	new_token->token = token_type;
	new_token->i = index;
	new_token->next = NULL;
	new_token->prev = NULL;
	return (new_token);
}

void	free_lexer_list(t_lexer *list)
{
	t_lexer	*temp;

	while (list)
	{
		temp = list;
		list = list->next;
		free(temp->str);
		free(temp);
	}
}

void	add_token(t_lexer **list, t_lexer *new_token)
{
	t_lexer	*temp;

	if (!*list)
	{
		*list = new_token;
		return ;
	}
	temp = *list;
	while (temp->next)
		temp = temp->next;
	temp->next = new_token;
	new_token->prev = temp;
}

t_tokens	check_token_type(char *str)
{
	if (!str)
		return (WORD);
	if (ft_strncmp(str, "|", 2) == 0)
		return (PIPE);
	if (ft_strncmp(str, "<", 2) == 0)
		return (REDIRECT_IN);
	if (ft_strncmp(str, "<<", 3) == 0)
		return (REDIRECT_IN);
	if (ft_strncmp(str, ">", 2) == 0)
		return (REDIRECT_OUT);
	if (ft_strncmp(str, ">>", 3) == 0)
		return (REDIRECT_OUT);
	return (WORD);
}

void	lexer(char *input)
{
	t_lexer		*token_list;
	char		**words;
	int			i;
	t_tokens	token_type;
	t_lexer		*temp;

	token_list = NULL;
	words = ft_split(input, ' ');
	i = -1;
	while (words[++i])
	{
		token_type = check_token_type(words[i]);
		add_token(&token_list, create_token(words[i], token_type, i));
	}
	i = 0;
	while (words[i])
		free(words[i++]);
	free(words);
	temp = token_list;
	while (temp)
	{
		printf("Token %d: %s (type: %d)\n", temp->i, temp->str, temp->token);
		temp = temp->next;
	}
	free_lexer_list(token_list);
}
