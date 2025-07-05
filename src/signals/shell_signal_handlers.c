#include "minishell.h"

extern sig_atomic_t	g_signal;

void	handle_signal_check(t_shell_data *shell)
{
	if (g_signal == SIGINT)
		set_exit_status(shell, 130);
	g_signal = 0;
}

void	handle_sigint_in_readline(t_shell_data *shell, char **input)
{
	set_exit_status(shell, 130);
	if (*input)
	{
		free(*input);
		*input = NULL;
	}
}

void	handle_sigint_after_readline(t_shell_data *shell, char *input)
{
	set_exit_status(shell, 130);
	free(input);
}

void	handle_sigint_in_parsing(t_shell_data *shell)
{
	set_exit_status(shell, 130);
	setup_signals(INTERACTIVE_MODE);
}
