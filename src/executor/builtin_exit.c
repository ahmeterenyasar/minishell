#include "minishell.h"

int	execute_exit(char **args, t_shell_data *shell)
{
	int		exit_code;
	long	temp;

	printf("exit\n");
	
	if (!args[1])
	{
		// No arguments - set exit flag and return special code
		set_exit_status(shell, get_exit_status(shell));
		return (-42); // Special return code to indicate exit
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
		return (-42); // Special return code to indicate exit
	}

	// Convert to number and handle overflow
	temp = ft_atoi(args[1]);
	exit_code = (int)(temp % 256);
	set_exit_status(shell, exit_code);
	return (-42); // Special return code to indicate exit
}