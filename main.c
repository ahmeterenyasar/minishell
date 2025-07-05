#include "minishell.h"

sig_atomic_t	g_signal = 0;

static t_shell_data	*initialize_shell(char **envp)
{
	t_shell_data	*shell;

	shell = init_shell_data(envp);
	if (!shell)
		return (NULL);
	setup_signals(INTERACTIVE_MODE);
	return (shell);
}

int	main(int argc, char **argv, char **envp)
{
	t_shell_data	*shell;

	(void)argc;
	(void)argv;
	shell = initialize_shell(envp);
	if (!shell)
		return (1);
	run_shell_main_loop(shell);
	free_shell_data(shell);
	return (0);
}
