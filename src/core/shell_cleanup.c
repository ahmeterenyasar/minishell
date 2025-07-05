#include "minishell.h"

static void	free_current_lines_if_exists(t_shell_data *shell)
{
	if (shell->current_lines)
	{
		free_str_array(shell->current_lines);
		shell->current_lines = NULL;
	}
}

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
	free_current_lines_if_exists(shell);
	free_envp_if_exists(shell);
	free(shell);
}
