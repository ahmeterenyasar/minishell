#include "minishell.h"

int	export_validate_and_report_error(char *arg)
{
	write(STDERR_FILENO, "export: `", 9);
	write(STDERR_FILENO, arg, ft_strlen(arg));
	write(STDERR_FILENO, "': not a valid identifier\n", 26);
	return (1);
}

int	export_report_memory_error(void)
{
	write(STDERR_FILENO, "export: memory allocation failed\n", 33);
	return (1);
}
