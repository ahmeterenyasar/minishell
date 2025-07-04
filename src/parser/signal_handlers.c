#include "minishell.h"
#include <termios.h>
#include <signal.h>

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
	rl_done = 1;
}
