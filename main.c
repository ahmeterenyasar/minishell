#include "minishell.h"

sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_command		*cmd;
	t_shell_data	*shell;
	int				last_exit_status;
	char			**lines;
	int				i;
	(void)argc;
	(void)argv;

	shell = init_shell_data(envp);
	if (!shell)
		return (1);

	setup_signals(INTERACTIVE_MODE);

	while (1)
	{
		// Check if CTRL+C was pressed and set exit status before resetting
		if (g_signal == SIGINT)
		{
			set_exit_status(shell, 130);
		}
		g_signal = 0;  // Reset signal flag before each prompt
		
		input = readline("minishell$ ");

		// Immediately check for CTRL+C after readline
		if (g_signal == SIGINT)
		{
			set_exit_status(shell, 130);
			if (input)
			{
				free(input);
				input = NULL;
			}
			continue;
		}

		// Handle Ctrl+D (EOF)
		if (!input)
		{
			printf("exit\n");
			break ;
		}

		// Check for CTRL+C after EOF handling
		if (g_signal == SIGINT)
		{
			set_exit_status(shell, 130);
			free(input);
			continue;
		}

		if (!*input)
		{
			free(input);
			continue;
		}

		add_history(input);

		lines = split_commands_by_newlines(input);
		free(input);
		if (!lines)
			continue;

		set_current_lines(shell, lines);  /* Use helper function for safer management */

		i = 0;
		while (lines[i])
		{
			if (*lines[i])
			{
				cmd = parse_input(lines[i], shell);
				if (cmd)
				{
					setup_signals(EXECUTING_MODE);
					execute_command(cmd, shell);
					setup_signals(INTERACTIVE_MODE);
					
					// Check if exit was called
					if (shell->should_exit)
					{
						free_command(cmd);
						clear_current_lines(shell);  /* Use helper function */
						clear_history();
						rl_clear_history();
						rl_cleanup_after_signal();
						last_exit_status = get_exit_status(shell);
						free_shell_data(shell);
						exit(last_exit_status);
					}
					
					free_command(cmd);
					cmd = NULL;
				}
				else 
				{
					// Handle parsing errors (including heredoc interruption)
					if (g_signal == SIGINT)
					{
						set_exit_status(shell, 130);
						// Make sure we're back in interactive mode
						setup_signals(INTERACTIVE_MODE);
					}
				}
			}
			i++;
		}
		clear_current_lines(shell);  /* Use helper function to clear lines */
		
		// Check for CTRL+C that might have been pressed after command execution
		if (g_signal == SIGINT)
		{
			set_exit_status(shell, 130);
		}
		
		// Ensure we're in interactive mode after processing commands
		setup_signals(INTERACTIVE_MODE);
	}

	// Cleanup for normal exit (Ctrl+D)
	clear_history();
	rl_clear_history();
	
	// Force cleanup of readline internal state
	rl_cleanup_after_signal();
	
	last_exit_status = get_exit_status(shell);
	free_shell_data(shell);
	return (last_exit_status);
}
