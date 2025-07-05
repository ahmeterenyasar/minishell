#include "minishell.h"

static void	free_envp_if_exists(t_shell_data *shell)
{
	if (shell->envp)
	{
		free_envp(shell->envp);
		shell->envp = NULL;
	}
}

void	free_shell_data(t_shell_data *shell)
{
	if (!shell)
		return ;
	cleanup_shell_readline_data(shell);
	free_envp_if_exists(shell);
	free(shell);
}
