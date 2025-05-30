#include "minishell.h"

void	free_envp(char **envp)
{
	int i = 0;

	if (!envp)
		return;

	while (envp[i])
	{
		free(envp[i]);  // Each string was malloc’d
		i++;
	}
	free(envp); // Top-level pointer
}


char	**copy_envp(char **envp)
{
	char	**new_envp;
	int		count;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	
	new_envp = malloc(sizeof(char *) * (count + 1));
	if (!new_envp)
		return (NULL);
	
	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (NULL);
		}
		i++;
	}
	new_envp[count] = NULL;
	return (new_envp);
}

int	find_env_var(char **envp, const char *name)
{
	int		i;
	int		name_len;
	
	if (!envp || !name)
		return (-1);
	
	name_len = ft_strlen(name);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, name_len) == 0 && 
			(envp[i][name_len] == '=' || envp[i][name_len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

char	**add_env_var(char **envp, const char *name, const char *value)
{
	char	**new_envp;
	char	*new_var;
	char	*temp;
	int		count;
	int		i;

	count = 0;
	while (envp[count])
		count++;
	
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (NULL);
	
	// Copy existing variables
	i = 0;
	while (i < count)
	{
		new_envp[i] = envp[i];
		i++;
	}
	
	// Create new variable string
	if (value)
	{
		temp = ft_strjoin(name, "=");
		if (!temp)
		{
			free(new_envp);
			return (NULL);
		}
		new_var = ft_strjoin(temp, value);
		free(temp);
	}
	else
		new_var = ft_strdup(name);
	
	if (!new_var)
	{
		free(new_envp);
		return (NULL);
	}
	
	new_envp[count] = new_var;
	new_envp[count + 1] = NULL;
	
	return (new_envp);
}

int	update_env_var(char **envp, const char *name, const char *value, int index)
{
	char	*new_var;
	char	*temp;
	
	// Create new variable string
	if (value)
	{
		temp = ft_strjoin(name, "=");
		if (!temp)
			return (-1);
		new_var = ft_strjoin(temp, value);
		free(temp);
	}
	else
		new_var = ft_strdup(name);
	
	if (!new_var)
		return (-1);
	
	free(envp[index]);
	envp[index] = new_var;
	return (0);
}

int	set_env_variable(t_shell_data *shell, const char *name, const char *value)
{
	int		index;
	char	**new_envp;

	index = find_env_var(shell->envp, name);
	if (index >= 0)
	{
		// Variable exists, update it in-place
		if (update_env_var(shell->envp, name, value, index) == -1)
			return (-1);
	}
	else
	{
		// Variable doesn't exist — create a new env array with one extra entry
		new_envp = add_env_var(shell->envp, name, value);
		if (!new_envp)
			return (-1);

		// Only free the old top-level pointer (NOT the strings inside)
		// The strings were reused in the new array
		free_envp(shell->envp);

		// Assign new envp
		shell->envp = new_envp;
	}
	return (0);
}
