#include "minishell.h"

sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_command		*cmd;
	t_token			*tokens;
	t_shell_data	*shell;

	(void)argc;
	(void)argv;

	shell = init_shell_data(envp);
	if (!shell)
		return (1);

	setup_signals(INTERACTIVE_MODE);

	while (1)
	{
		g_signal = 0;
		input = readline("minishell$ ");

		if (!input) // Ctrl+D (EOF)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);

		// print tokens
		tokens = tokenize(input);
		if (tokens)
		{
			print_tokens(tokens);
			free_tokens(tokens);
		}
		cmd = parse_input(input, shell);
		if (cmd)
		{
			setup_signals(EXECUTING_MODE);
			// execute_command(cmd, shell); // Pass shell to executor
			print_command(cmd);
			setup_signals(INTERACTIVE_MODE);
			free_command(cmd);
		}
		free(input);
	}

	rl_clear_history();
	free_shell_data(shell);
	return (0);
}
