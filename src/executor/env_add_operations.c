#include "minishell.h"

static int	count_env_variables(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static void	cleanup_failed_add_env(char **new_envp, int count)
{
	while (--count >= 0)
		free(new_envp[count]);
	free(new_envp);
}

char	**add_env_var(char **envp, const char *name, const char *value)
{
	char	**new_envp;
	char	*new_var;
	int		count;

	count = count_env_variables(envp);
	new_envp = copy_existing_env_vars(envp, count);
	if (!new_envp)
		return (NULL);
	new_var = create_env_variable(name, value);
	if (!new_var)
	{
		cleanup_failed_add_env(new_envp, count);
		return (NULL);
	}
	new_envp[count] = new_var;
	new_envp[count + 1] = NULL;
	return (new_envp);
}

static char	*create_updated_variable(const char *name, const char *value)
{
	char	*new_var;
	char	*temp;

	if (value)
	{
		temp = ft_strjoin(name, "=");
		if (!temp)
			return (NULL);
		new_var = ft_strjoin(temp, value);
		free(temp);
	}
	else
		new_var = ft_strdup(name);
	return (new_var);
}

int	update_env_var(char **envp, const char *name, const char *value, int index)
{
	char	*new_var;

	new_var = create_updated_variable(name, value);
	if (!new_var)
		return (-1);
	free(envp[index]);
	envp[index] = new_var;
	return (0);
}
