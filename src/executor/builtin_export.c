#include "minishell.h"

int	export_without_value(t_shell_data *shell, const char *name)
{
	int	index;
	
	index = find_env_var(shell->envp, name);
	if (index >= 0)
		return (0);
	return (set_env_variable(shell, name, NULL));
}

static int	handle_export_variable(char *arg, t_shell_data *shell)
{
	if (!is_valid_identifier(arg))
	{
		write(STDERR_FILENO, "export: `", 9);
		write(STDERR_FILENO, arg, ft_strlen(arg));
		write(STDERR_FILENO, "': not a valid identifier\n", 26);
		return (1);
	}
	if (export_without_value(shell, arg) == -1)
	{
		write(STDERR_FILENO, "export: memory allocation failed\n", 33);
		return (1);
	}
	return (0);
}

static int	process_export_argument(char *arg, t_shell_data *shell)
{
	if (ft_strchr(arg, '='))
		return (handle_export_assignment(arg, shell));
	return (handle_export_variable(arg, shell));
}

static void	process_export_no_args(t_shell_data *shell)
{
	print_export_vars(shell->envp);
	set_exit_status(shell, 0);
}

int	execute_export(char **args, t_shell_data *shell)
{
	int	i;
	int	ret_status;

	if (!args[1])
	{
		process_export_no_args(shell);
		return (0);
	}
	ret_status = 0;
	i = 1;
	while (args[i])
	{
		ret_status |= process_export_argument(args[i], shell);
		i++;
	}
	set_exit_status(shell, ret_status);
	return (ret_status);
}
