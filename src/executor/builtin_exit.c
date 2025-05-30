#include "minishell.h"

int	execute_exit(char **args, t_shell_data *shell)
{
	int		exit_code;
	long	temp;

	printf("exit\n");
	
	if (!args[1])
	{
		// No arguments - exit with last exit status
		exit_code = get_exit_status(shell);
		free_shell_data(shell);
		exit(exit_code);
	}

	// Too many arguments
	if (args[2])
	{
		write(STDERR_FILENO, "exit: too many arguments\n", 25);
		set_exit_status(shell, 1);
		return (1);
	}

	// Check if argument is numeric
	if (!is_numeric_string(args[1]))
	{
		write(STDERR_FILENO, "exit: ", 6);
		write(STDERR_FILENO, args[1], ft_strlen(args[1]));
		write(STDERR_FILENO, ": numeric argument required\n", 28);
		free_shell_data(shell);
		exit(2);
	}

	// Convert to number and handle overflow
	temp = ft_atoi(args[1]);
	exit_code = (int)(temp % 256); // Wrap around like bash
	
	free_shell_data(shell);
	exit(exit_code);
}
