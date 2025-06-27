#include "minishell.h"

char	**sort_envp_for_export(char **envp)
{
	char	**sorted;
	char	*temp;
	int		count;

	int i, j;
	// Count variables
	count = 0;
	while (envp[count])
		count++;
	// Copy envp
	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return (NULL);
	i = 0;
	while (i < count)
	{
		sorted[i] = envp[i];
		i++;
	}
	sorted[count] = NULL;
	// Bubble sort
	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (ft_strcmp(sorted[j], sorted[j + 1]) > 0)
			{
				temp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
	return (sorted);
}

void	print_export_vars(char **envp)
{
	char	**sorted;
	char	*equals;
	int		i;

	sorted = sort_envp_for_export(envp);
	if (!sorted)
	{
		// Fallback to unsorted if malloc fails
		sorted = envp;
	}
	i = 0;
	while (sorted[i])
	{
		write(STDOUT_FILENO, "declare -x ", 11);
		equals = ft_strchr(sorted[i], '=');
		if (equals)
		{
			// Print name=value format with quotes around value
			*equals = '\0';
			write(STDOUT_FILENO, sorted[i], ft_strlen(sorted[i]));
			write(STDOUT_FILENO, "=\"", 2);
			write(STDOUT_FILENO, equals + 1, ft_strlen(equals + 1));
			write(STDOUT_FILENO, "\"\n", 2);
			*equals = '=';
		}
		else
		{
			// Just the variable name (exported but no value)
			write(STDOUT_FILENO, sorted[i], ft_strlen(sorted[i]));
			write(STDOUT_FILENO, "\n", 1);
		}
		i++;
	}
	if (sorted != envp)
		free(sorted);
}
