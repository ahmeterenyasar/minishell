#include "minishell.h"

t_shell_data	*init_shell_data(char **envp)
{
	t_shell_data	*shell;

	shell = malloc(sizeof(t_shell_data));
	if (!shell)
		return (NULL);
	shell->last_exit_status = 0;
	shell->envp = envp;
	return (shell);
}

void	set_exit_status(t_shell_data *shell, int status)
{
	if (!shell)
		return ;
	shell->last_exit_status = status;
}

int	get_exit_status(t_shell_data *shell)
{
	if (!shell)
		return (0);
	return (shell->last_exit_status);
}

void	free_shell_data(t_shell_data *shell)
{
	if (!shell)
		return ;
	free(shell);
}