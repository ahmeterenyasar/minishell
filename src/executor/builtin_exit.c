#include "minishell.h"

int	execute_exit(char **args, t_shell_data *shell)
{
	int exit_code;
	long temp;

	write(STDOUT_FILENO, "exit\n", 5);

	if (!args[1])
	{
		// No arguments - use current exit status
		shell->should_exit = 1;
		return (get_exit_status(shell));
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
		set_exit_status(shell, 2);
		shell->should_exit = 1;
		return (2);
	}

	// Convert to number and handle overflow
	temp = ft_atoi(args[1]);
	exit_code = (int)temp;
	set_exit_status(shell, exit_code);
	shell->should_exit = 1;
	return (exit_code);
}