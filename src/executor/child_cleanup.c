#include "minishell.h"

void	cleanup_readline_history(void)
{
	clear_history();
	rl_clear_history();
	rl_cleanup_after_signal();
}

void	cleanup_shell_lines(t_shell_data *shell)
{
	if (shell && shell->current_lines)
	{
		free_str_array(shell->current_lines);
		shell->current_lines = NULL;
	}
}

void	cleanup_shell_environment(t_shell_data *shell)
{
	if (shell && shell->envp)
	{
		free_envp(shell->envp);
	}
}

void	cleanup_child_inherited_memory(t_shell_data *shell)
{
	cleanup_readline_history();
	cleanup_shell_lines(shell);
	cleanup_shell_environment(shell);
	if (shell)
	{
		free(shell);
	}
}

void	exit_with_cleanup(t_shell_data *shell, t_command *cmd, int exit_code)
{
	cleanup_child_inherited_memory(shell);
	free_command(cmd);
	exit(exit_code);
}
