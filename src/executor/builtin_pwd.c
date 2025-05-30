#include "minishell.h"

/* Helper function to find environment variable index */
static int	find_env_index(char **envp, const char *name)
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
			envp[i][name_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/* Helper function to set environment variable */
static int	set_env_var(t_shell_data *shell, const char *name, const char *value)
{
	char	*new_var;
	char	*temp;
	int		index;
	char	**new_envp;
	int		count;
	int		i;

	// Create new variable string: name=value
	temp = ft_strjoin(name, "=");
	if (!temp)
		return (-1);
	new_var = ft_strjoin(temp, value);
	free(temp);
	if (!new_var)
		return (-1);

	index = find_env_index(shell->envp, name);
	if (index >= 0)
	{
		// Variable exists, replace it
		free(shell->envp[index]);
		shell->envp[index] = new_var;
		return (0);
	}

	// Variable doesn't exist, add it
	count = 0;
	while (shell->envp[count])
		count++;

	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
	{
		free(new_var);
		return (-1);
	}

	i = 0;
	while (i < count)
	{
		new_envp[i] = shell->envp[i];
		i++;
	}
	new_envp[count] = new_var;
	new_envp[count + 1] = NULL;

	free(shell->envp);
	shell->envp = new_envp;
	return (0);
}

/* PWD command implementation */
int	execute_pwd(t_shell_data *shell)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		set_exit_status(shell, 1);
		return (1);
	}
	printf("%s\n", cwd);
	free(cwd);
	set_exit_status(shell, 0);
	return (0);
}

/* Update PWD and OLDPWD environment variables */
void	update_pwd_vars(t_shell_data *shell, char *old_pwd)
{
	char	*new_pwd;
	char	*current_pwd;
	
	if (!shell)
		return;
	
	new_pwd = getcwd(NULL, 0);
	if (!new_pwd)
	{
		// If getcwd fails, we can't update PWD
		// This can happen if the directory was deleted
		return;
	}
	
	// Get current PWD value for OLDPWD
	current_pwd = get_env_value("PWD", shell);
	
	// Set OLDPWD to the previous PWD value, or old_pwd if PWD wasn't set
	if (current_pwd && *current_pwd)
	{
		set_env_var(shell, "OLDPWD", current_pwd);
		free(current_pwd);
	}
	else if (old_pwd && *old_pwd)
	{
		set_env_var(shell, "OLDPWD", old_pwd);
	}
	
	// Set PWD to the new current directory
	set_env_var(shell, "PWD", new_pwd);
	
	free(new_pwd);
}
