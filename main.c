#include "minishell.h"

int	main(void)
{
	char	*input;
	char	*shell_name;

	shell_name = "minihell> ";
	while (1)
	{
		input = readline(shell_name);
		if (!input)
			break ;
		if (ft_strncmp(input, "exit", 4) == 0)
		{
			free(input);
			break ;
		}
		if (ft_strlen(input) > 0)
			add_history(input);
		parser(input);
		free(input);
	}
	rl_clear_history();
}
