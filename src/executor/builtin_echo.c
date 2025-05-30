#include "minishell.h"

/* Echo command implementation */
int	is_valid_n_flag(const char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);

	i = 2;
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

int	execute_echo(char **args, t_shell_data *shell)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;

	// Process all consecutive valid -n flags
	while (args[i] && is_valid_n_flag(args[i]))
	{
		newline = 0;
		i++;
	}

	// Print remaining arguments
	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}

	if (newline)
		printf("\n");

	set_exit_status(shell, 0);
	return (0);
}
