#include "minishell.h"

static int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static int	copy_env_vars(t_shell_data *shell, char **envp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		shell->envp[i] = ft_strdup(envp[i]);
		if (!shell->envp[i])
		{
			while (--i >= 0)
				free(shell->envp[i]);
			free(shell->envp);
			return (0);
		}
		i++;
	}
	shell->envp[count] = NULL;
	return (1);
}

static t_shell_data	*allocate_shell_structure(void)
{
	t_shell_data	*shell;

	shell = malloc(sizeof(t_shell_data));
	if (!shell)
		return (NULL);
	shell->last_exit_status = 0;
	shell->current_lines = NULL;
	shell->should_exit = 0;
	return (shell);
}

static int	allocate_env_array(t_shell_data *shell, int count)
{
	shell->envp = malloc(sizeof(char *) * (count + 1));
	if (!shell->envp)
	{
		free(shell);
		return (0);
	}
	return (1);
}

t_shell_data	*init_shell_data(char **envp)
{
	t_shell_data	*shell;
	int				count;

	shell = allocate_shell_structure();
	if (!shell)
		return (NULL);
	count = count_env_vars(envp);
	if (!allocate_env_array(shell, count))
		return (NULL);
	if (!copy_env_vars(shell, envp, count))
	{
		free(shell);
		return (NULL);
	}
	return (shell);
}
