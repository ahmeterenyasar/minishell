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

/* Export command implementation */
int	execute_export(char **args, t_shell_data *shell)
{
	int		i;
	char	*temp;
	char	*equals;
	char	*name;
	char	*value;
	int		ret_status;

	// No arguments - show all exported variables
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
		// Check if this is a variable assignment (contains '=')
		if (ft_strchr(args[i], '='))
		{
			// Variable assignment: VAR=value
			// Create a safe copy to modify
			temp = ft_strdup(args[i]);
			if (!temp)
			{
				write(STDERR_FILENO, "export: memory allocation failed\n", 33);
				ret_status = 1;
				i++;
				continue;
			}
			
			equals = ft_strchr(temp, '=');
			if (!equals)  // This shouldn't happen, but safety check
			{
				free(temp);
				i++;
				continue;
			}
			
			*equals = '\0';
			name = temp;
			value = equals + 1;
			
			if (!is_valid_identifier(name))
			{
				write(STDERR_FILENO, "export: `", 9);
				write(STDERR_FILENO, args[i], ft_strlen(args[i]));
				write(STDERR_FILENO, "': not a valid identifier\n", 26);
				ret_status = 1;
				free(temp);
				i++;
				continue;
			}
			
			if (set_env_variable(shell, name, value) == -1)
			{
				write(STDERR_FILENO, "export: memory allocation failed\n", 33);
				ret_status = 1;
			}
			
			free(temp);
		}
		else
		{
			// Just export existing variable: export VAR
			if (!is_valid_identifier(args[i]))
			{
				write(STDERR_FILENO, "export: `", 9);
				write(STDERR_FILENO, args[i], ft_strlen(args[i]));
				write(STDERR_FILENO, "': not a valid identifier\n", 26);
				ret_status = 1;
				i++;
				continue;
			}
			
			if (export_without_value(shell, args[i]) == -1)
			{
				write(STDERR_FILENO, "export: memory allocation failed\n", 33);
				ret_status = 1;
			}
		}
		i++;
	}

	set_exit_status(shell, ret_status);
	return (ret_status);
}
