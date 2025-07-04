#include "minishell.h"

static int	validate_and_report_error(char *arg)
{
	write(STDERR_FILENO, "export: `", 9);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
	return (1);
}

static int	report_memory_error(void)
{
	write(STDERR_FILENO, "export: memory allocation failed\n", 33);
	return (1);
}

static char	*duplicate_argument(char *arg)
{
	char	*temp;

	temp = ft_strdup(arg);
	if (!temp)
		return (NULL);
	return (temp);
}

static int	process_assignment_validation(char *name, char *arg)
{
	if (!is_valid_identifier(name))
		return (validate_and_report_error(arg));
	return (0);
}

int	handle_export_assignment(char *arg, t_shell_data *shell)
{
	char	*temp;
	char	*equals;
	char	*name;
	char	*value;

	temp = duplicate_argument(arg);
	if (!temp)
		return (report_memory_error());
	equals = ft_strchr(temp, '=');
	if (!equals)
	{
		free(temp);
		return (0);
	}
	*equals = '\0';
	name = temp;
	value = equals + 1;
	if (process_assignment_validation(name, arg))
	{
		free(temp);
		return (1);
	}
	if (set_env_variable(shell, name, value) == -1)
	{
		free(temp);
		return (report_memory_error());
	}
	free(temp);
	return (0);
}
