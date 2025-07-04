#include "minishell.h"

static int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static char	**copy_env_array(char **envp, int count)
{
	char	**sorted;
	int		i;

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
	return (sorted);
}

static void	bubble_sort_env(char **sorted, int count)
{
	char	*temp;
	int		i;
	int		j;

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
}

char	**sort_envp_for_export(char **envp)
{
	char	**sorted;
	int		count;

	count = count_env_vars(envp);
	sorted = copy_env_array(envp, count);
	if (!sorted)
		return (NULL);
	bubble_sort_env(sorted, count);
	return (sorted);
}

static void	print_export_var_with_value(char *var)
{
	char	*equals;

	write(STDOUT_FILENO, "declare -x ", 11);
	equals = ft_strchr(var, '=');
	*equals = '\0';
	write(STDOUT_FILENO, var, ft_strlen(var));
	write(STDOUT_FILENO, "=\"", 2);
	write(STDOUT_FILENO, equals + 1, ft_strlen(equals + 1));
	write(STDOUT_FILENO, "\"\n", 2);
	*equals = '=';
}

static void	print_export_var_without_value(char *var)
{
	write(STDOUT_FILENO, "declare -x ", 11);
	write(STDOUT_FILENO, var, ft_strlen(var));
	write(STDOUT_FILENO, "\n", 1);
}

void	print_export_vars(char **envp)
{
	char	**sorted;
	char	*equals;
	int		i;

	sorted = sort_envp_for_export(envp);
	if (!sorted)
		sorted = envp;
	i = 0;
	while (sorted[i])
	{
		equals = ft_strchr(sorted[i], '=');
		if (equals)
			print_export_var_with_value(sorted[i]);
		else
			print_export_var_without_value(sorted[i]);
		i++;
	}
	if (sorted != envp)
		free(sorted);
}
