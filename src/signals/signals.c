#include "minishell.h"

void	setup_signals(int context)
{
	if (context == INTERACTIVE_MODE)
		setup_signals_interactive();
	else if (context == EXECUTING_MODE)
		setup_signals_executing();
	else if (context == HEREDOC_MODE)
		setup_signals_heredoc();
}
