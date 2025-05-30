#include "minishell.h"

/* Helper function to find environment variable index */
static int	find_env_var_index(char **envp, const char *name)
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

/* Helper function to count environment variables */
static int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

/* Remove a variable from the environment array */
/* FIXME NEEDS UPDATE free_envp causes munmap_chunk():
 invalid pointer IOT instruction core dump*/
int	remove_env_var(t_shell_data *shell, const char *name)
{
	int		index;
	int		count;
	int		i, j;
	char	**new_envp;

	index = find_env_var_index(shell->envp, name);
	if (index == -1)
		return (0);

	count = count_env_vars(shell->envp);

	new_envp = malloc(sizeof(char *) * count); // one less + NULL
	if (!new_envp)
		return (-1);

	i = 0;
	j = 0;
	while (shell->envp[i])
	{
		if (i != index)
		{
			new_envp[j] = ft_strdup(shell->envp[i]);
			if (!new_envp[j])
			{
				while (j-- > 0)
					free(new_envp[j]);
				free(new_envp);
				return (-1);
			}
			j++;
		}
		else
		{
			free(shell->envp[i]); // remove this one
		}
		i++;
	}
	new_envp[j] = NULL;

	free_envp(shell->envp); // free old full envp
	shell->envp = new_envp;

	return (0);
}


/* Unset command implementation */
int	execute_unset(char **args, t_shell_data *shell)
{
	int	i;
	int	ret_status;

	// No arguments - success (bash behavior)
	if (!args[1])
	{
		set_exit_status(shell, 0);
		return (0);
	}

	ret_status = 0;
	i = 1;
	while (args[i])
	{
		// Validate the variable name
		if (!is_valid_identifier(args[i]))
		{
			write(STDERR_FILENO, "unset: `", 8);
			write(STDERR_FILENO, args[i], ft_strlen(args[i]));
			write(STDERR_FILENO, "': not a valid identifier\n", 26);
			ret_status = 1;
			i++;
			continue;
		}
		
		// Remove the variable from environment
		if (remove_env_var(shell, args[i]) == -1)
		{
			write(STDERR_FILENO, "unset: memory allocation failed\n", 32);
			ret_status = 1;
		}
		
		i++;
	}

	set_exit_status(shell, ret_status);
	return (ret_status);
}
