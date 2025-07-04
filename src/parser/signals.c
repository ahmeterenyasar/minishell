#include "minishell.h"

/**
 * Set up signal handlers based on the current context
 * This function serves as a unified interface for signal setup
 */
void	setup_signals(int context)
{
	if (context == INTERACTIVE_MODE)
		setup_signals_interactive();
	else if (context == EXECUTING_MODE)
		setup_signals_executing();
	else if (context == HEREDOC_MODE)
		setup_signals_heredoc();
}
