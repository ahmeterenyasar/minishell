#include "minishell.h"

int	export_without_value(t_shell_data *shell, const char *name)
{
	int	index;
	
	index = find_env_var(shell->envp, name);
	if (index >= 0)
	{
		// Variable already exists, just mark as exported (it already is in our envp)
		return (0);
	}
	else
	{
		// Variable doesn't exist, create it without value
		return (set_env_variable(shell, name, NULL));
	}
}

static int	handle_export_assignment(char *arg, t_shell_data *shell)
{
	char	*temp;
	char	*equals;
	char	*name;
	char	*value;

	temp = ft_strdup(arg);
	if (!temp)
	{
		write(STDERR_FILENO, "export: memory allocation failed\n", 33);
		return (1);
	}
	equals = ft_strchr(temp, '=');
	if (!equals)
	{
		free(temp);
		return (0);
	}
	*equals = '\0';
	name = temp;
	value = equals + 1;
	if (!is_valid_identifier(name))
	{
		write(STDERR_FILENO, "export: `", 9);
		write(STDERR_FILENO, arg, ft_strlen(arg));
		write(STDERR_FILENO, "': not a valid identifier\n", 26);
		free(temp);
		return (1);
	}
	if (set_env_variable(shell, name, value) == -1)
	{
		write(STDERR_FILENO, "export: memory allocation failed\n", 33);
		free(temp);
		return (1);
	}
	free(temp);
	return (0);
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

/* Export command implementation */
int	execute_export(char **args, t_shell_data *shell)
{
	int		i;
	int		ret_status;

	if (!args[1])
	{
		print_export_vars(shell->envp);
		set_exit_status(shell, 0);
		return (0);
	}
	ret_status = 0;
	i = 1;
	while (args[i])
	{
		if (ft_strchr(args[i], '='))
			ret_status |= handle_export_assignment(args[i], shell);
		else
			ret_status |= handle_export_variable(args[i], shell);
		i++;
	}
	set_exit_status(shell, ret_status);
	return (ret_status);
}
