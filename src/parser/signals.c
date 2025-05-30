#include "minishell.h"

/**
 * Signal handler for SIGINT (Ctrl+C) in interactive mode
 */
void	handle_sigint_interactive(int signo)
{
	(void)signo;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * Signal handler for SIGINT (Ctrl+C) in executing mode
 */
void	handle_sigint_executing(int signo)
{
	(void)signo;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
}

/**
 * Signal handler for SIGQUIT (Ctrl+\) in executing mode
 */
void	handle_sigquit_executing(int signo)
{
	(void)signo;
	g_signal = SIGQUIT;
	write(STDOUT_FILENO, "Quit (core dumped)\n", 19);
}

/**
 * Signal handler for SIGINT (Ctrl+C) in heredoc mode
 */
void	handle_sigint_heredoc(int signo)
{
	(void)signo;
	g_signal = SIGINT;
	write(STDOUT_FILENO, "\n", 1);
	close(STDIN_FILENO);
	// Force readline to return NULL by closing stdin temporarily
	// This will be handled in heredoc processing
}

/**
 * Set up signal handlers based on the current context
 */
static void	set_signal(int sig, void (*handler)(int))
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handler;
	sigaction(sig, &sa, NULL);
}

void	setup_signals(int context)
{
	if (context == INTERACTIVE_MODE)
	{
		set_signal(SIGINT, handle_sigint_interactive);
		set_signal(SIGQUIT, SIG_IGN);
	}
	else if (context == EXECUTING_MODE)
	{
		set_signal(SIGINT, handle_sigint_executing);
		set_signal(SIGQUIT, handle_sigquit_executing);
	}
	else if (context == HEREDOC_MODE)
	{
		set_signal(SIGINT, handle_sigint_heredoc);
		set_signal(SIGQUIT, SIG_IGN);
	}
}

void	reset_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = SIG_DFL;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}