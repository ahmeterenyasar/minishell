#include "minishell.h"

/* Export helper functions */
int	is_valid_identifier(const char *name)
{
	int	i;

	if (!name || !*name)
		return (0);
	
	// First character must be alphabetic or underscore
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (0);
	
	// Rest can be alphanumeric or underscore
	i = 1;
	while (name[i])
	{
		if (!ft_isalnum(name[i]) && name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* Exit command implementation */
int	is_numeric_string(const char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	
	i = 0;
	if (str[0] == '+' || str[0] == '-')
		i = 1;
	
	if (!str[i]) // Just a sign character
		return (0);
		
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}