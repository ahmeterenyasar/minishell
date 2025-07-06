#include "minishell.h"

void	set_exit_status(t_shell_data *shell, int status)
{
	if (!shell)
		return ;
	/* Ensure exit status is in valid range (0-255) like bash */
	shell->last_exit_status = status & 0xFF;
}

int	get_exit_status(t_shell_data *shell)
{
	if (!shell)
		return (0);
	return (shell->last_exit_status);
}
