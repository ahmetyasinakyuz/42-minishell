#include "parser.h"

void	parser(char *input)
{
	char	**commands;
	int		i;

	commands = ft_split(input, ' ');
	i = 0;
	while (commands[i])
	{
		printf("command: %s\n", commands[i]);
		i++;
	}
}
