#include "minishell.h"

static int	update_existing_env_var(t_shell_data *shell, const char *name,
		const char *value, int index)
{
	if (update_env_var(shell->envp, name, value, index) == -1)
		return (-1);
	return (0);
}

static int	add_new_env_var(t_shell_data *shell, const char *name,
		const char *value)
{
	char	**new_envp;
	char	**old_envp;

	old_envp = shell->envp;
	new_envp = add_env_var(shell->envp, name, value);
	if (!new_envp)
		return (-1);
	shell->envp = new_envp;
	free_envp(old_envp);
	return (0);
}

int	set_env_variable(t_shell_data *shell, const char *name, const char *value)
{
	int	index;

	index = find_env_var(shell->envp, name);
	if (index >= 0)
		return (update_existing_env_var(shell, name, value, index));
	else
		return (add_new_env_var(shell, name, value));
}
