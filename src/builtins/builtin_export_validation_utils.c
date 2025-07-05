#include "minishell.h"

char	*export_duplicate_argument(char *arg)
{
	char	*temp;

	temp = ft_strdup(arg);
	if (!temp)
		return (NULL);
	return (temp);
}

int	export_validate_identifier(char *name, char *arg)
{
	if (!is_valid_identifier(name))
		return (export_validate_and_report_error(arg));
	return (0);
}

int	export_check_assignment_format(char *arg)
{
	char	*equals;

	equals = ft_strchr(arg, '=');
	if (!equals)
		return (0);
	return (1);
}
