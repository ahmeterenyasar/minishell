#include "minishell.h"

/* Environment display */
void	print_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		write(STDOUT_FILENO, envp[i], ft_strlen(envp[i]));
		write(STDOUT_FILENO, "\n", 1);
		i++;
	}
}

/* ENV command implementation */
int	execute_env(char **args, t_shell_data *shell)
{
	if (args[1])
	{
		write(STDERR_FILENO, "env: too many arguments\n", 24);
		set_exit_status(shell, 1);
		return (1);
	}
	print_env(shell->envp);
	set_exit_status(shell, 0);
	return (0);
}
