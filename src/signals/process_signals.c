#include "minishell.h"

int	handle_signal_termination(int status, t_shell_data *shell)
{
	int	signal_num;

	if (!WIFSIGNALED(status))
		return (0);
	signal_num = WTERMSIG(status);
	if (signal_num == SIGINT)
	{
		set_exit_status(shell, 130);
		return (130);
	}
	if (signal_num == SIGQUIT)
	{
		set_exit_status(shell, 131);
		return (131);
	}
	set_exit_status(shell, 128 + signal_num);
	return (128 + signal_num);
}

int	handle_global_signals(t_shell_data *shell)
{
	if (g_signal == SIGINT)
	{
		set_exit_status(shell, 130);
		g_signal = 0;  /* Reset signal after handling */
		return (130);
	}
	if (g_signal == SIGQUIT)
	{
		set_exit_status(shell, 131);
		g_signal = 0;  /* Reset signal after handling */
		return (131);
	}
	return (0);
}

int	check_heredoc_interruption(t_shell_data *shell)
{
	if (g_signal == SIGINT)
	{
		set_exit_status(shell, 130);
		g_signal = 0;  /* Reset signal after handling */
		return (130);
	}
	return (0);
}

void	reset_global_signal(void)
{
	g_signal = 0;
}
