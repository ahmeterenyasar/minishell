#include "minishell.h"

static int	count_env_variables(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static int	copy_env_without_target(char **old_envp, char **new_envp,
		int skip_index)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (old_envp[i])
	{
		if (i != skip_index)
		{
			new_envp[j] = ft_strdup(old_envp[i]);
			if (!new_envp[j])
				return (j);
			j++;
		}
		i++;
	}
	new_envp[j] = NULL;
	return (-1);
}

static void	cleanup_failed_copy(char **new_envp, int failed_at)
{
	while (failed_at-- > 0)
		free(new_envp[failed_at]);
	free(new_envp);
}

char	**create_env_without_var(char **envp, int skip_index)
{
	char	**new_envp;
	int		count;
	int		failed_at;

	count = count_env_variables(envp);
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return (NULL);
	failed_at = copy_env_without_target(envp, new_envp, skip_index);
	if (failed_at != -1)
	{
		cleanup_failed_copy(new_envp, failed_at);
		return (NULL);
	}
	return (new_envp);
}
