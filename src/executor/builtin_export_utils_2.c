#include  "minishell.h"

char	**sort_envp_for_export(char **envp)
{
	char	**sorted;
	char	*temp;
	int		count;
	int		i, j;
	
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
	
	// Simple bubble sort
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
		printf("declare -x ");
		equals = ft_strchr(sorted[i], '=');
		if (equals)
		{
			// Print name=value format with quotes around value
			*equals = '\0';
			printf("%s=\"%s\"\n", sorted[i], equals + 1);
			*equals = '=';
		}
		else
		{
			// Just the variable name (exported but no value)
			printf("%s\n", sorted[i]);
		}
		i++;
	}

	if (sorted != envp)
		free(sorted);
}
