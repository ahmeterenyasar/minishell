#include "minishell.h"

int	execute_command(t_command *cmd, t_shell_data *shell)
{
	int	result;
	int	heredoc_result;

	if (!cmd)
		return (0);
	reset_global_signal();
	heredoc_result = process_heredocs_with_signal_check(cmd, shell);
	if (heredoc_result > 0)
		return (heredoc_result);
	result = execute_pipeline(cmd, shell);
	heredoc_result = handle_global_signals(shell);
	if (heredoc_result > 0)
		return (heredoc_result);
	if (shell->should_exit)
		return (result);
	return (result);
}
