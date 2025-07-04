#include "minishell.h"

int	remove_env_variable(t_shell_data *shell, const char *name)
{
	int		index;
	char	**new_envp;

	index = find_env_var(shell->envp, name);
	if (index == -1)
		return (0);
	new_envp = create_env_without_var(shell->envp, index);
	if (!new_envp)
		return (-1);
	free_envp(shell->envp);
	shell->envp = new_envp;
	return (0);
}

static int	process_unset_argument(t_shell_data *shell, const char *arg)
{
	if (!is_valid_identifier(arg))
		return (0);
	if (remove_env_variable(shell, arg) == -1)
	{
		write(STDERR_FILENO, "unset: memory allocation failed\n", 32);
		return (-1);
	}
	return (0);
}

int	process_unset_args(t_shell_data *shell, char **args)
{
	int	i;
	int	ret_status;

	ret_status = 0;
	i = 1;
	while (args[i])
	{
		if (process_unset_argument(shell, args[i]) == -1)
			ret_status = 1;
		i++;
	}
	return (ret_status);
}
