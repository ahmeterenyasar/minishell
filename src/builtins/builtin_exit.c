#include "minishell.h"

static int	handle_exit_no_args(t_shell_data *shell)
{
	shell->should_exit = 1;
	return (get_exit_status(shell));
}

static int	handle_exit_too_many_args(t_shell_data *shell)
{
	write(STDERR_FILENO, "exit: too many arguments\n", 25);
	set_exit_status(shell, 1);
	return (1);
}

static int	handle_exit_invalid_arg(char *arg, t_shell_data *shell)
{
	write(STDERR_FILENO, "exit: ", 6);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, ": numeric argument required\n", 28);
	set_exit_status(shell, 2);
	shell->should_exit = 1;
	return (2);
}

static int	handle_exit_with_code(char *arg, t_shell_data *shell)
{
	long	temp;
	int		exit_code;

	temp = ft_atoi(arg);
	exit_code = (int)temp;
	set_exit_status(shell, exit_code);
	shell->should_exit = 1;
	return (exit_code);
}

int	execute_exit(char **args, t_shell_data *shell)
{
	write(STDOUT_FILENO, "exit\n", 5);
	if (!args[1])
		return (handle_exit_no_args(shell));
	if (args[2])
		return (handle_exit_too_many_args(shell));
	if (!is_numeric_string(args[1]))
		return (handle_exit_invalid_arg(args[1], shell));
	return (handle_exit_with_code(args[1], shell));
}
