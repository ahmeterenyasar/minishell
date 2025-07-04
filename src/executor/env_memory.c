#include "minishell.h"

void	free_envp(char **envp)
{
	int i;

	if (!envp)
		return;
	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

static int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static char	**allocate_env_array(int count)
{
	char	**new_envp;

	new_envp = malloc(sizeof(char *) * (count + 1));
	return (new_envp);
}

static void	free_partial_envp(char **envp, int up_to)
{
	while (--up_to >= 0)
		free(envp[up_to]);
	free(envp);
}

char	**copy_envp(char **envp)
{
	char	**new_envp;
	int		count;
	int		i;

	count = count_env_vars(envp);
	new_envp = allocate_env_array(count);
	if (!new_envp)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			free_partial_envp(new_envp, i);
			return (NULL);
		}
		i++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}
