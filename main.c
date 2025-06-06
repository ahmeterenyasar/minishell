#include "minishell.h"

sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_command		*cmd;
	t_shell_data	*shell;
	int				last_exit_status;
	int				exec_result;
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
		g_signal = 0;  // Reset signal flag before each prompt
		input = readline("minishell$ ");

		// Handle Ctrl+D (EOF)
		if (!input)
		{
			printf("exit\n");
			break ;
		}

		// Handle Ctrl+C during readline
		if (g_signal == SIGINT)
		{
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

		i = 0;
		while (lines[i])
		{
			if (*lines[i])
			{
				cmd = parse_input(lines[i], shell);
				if (cmd)
				{
					setup_signals(EXECUTING_MODE);
					exec_result = execute_command(cmd, shell);
					setup_signals(INTERACTIVE_MODE);
					
					// Check if exit was called (special return code -42)
					if (exec_result == -42)
					{
						free_command(cmd);
						free_str_array(lines);
						clear_history();
						rl_clear_history();
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
		free_str_array(lines);
		lines = NULL;
		
		// Ensure we're in interactive mode after processing commands
		setup_signals(INTERACTIVE_MODE);
	}

	// Cleanup for normal exit (Ctrl+D)
	clear_history();
	rl_clear_history();
	last_exit_status = get_exit_status(shell);
	free_shell_data(shell);
	return (last_exit_status);
}
