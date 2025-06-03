#include "minishell.h"

sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_command		*cmd;
	t_shell_data	*shell;
	int				last_exit_status;
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

		if (!*input)
		{
			free(input);
			continue;
		}

		add_history(input);

		char **lines = split_commands_by_newlines(input);
		free(input); // ✅ Input freed early
		if (!lines)
			continue;

		int i = 0;
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
					free_command(cmd); // ✅ Command freed
					cmd = NULL; // ✅ Pointer nulled
				}
				else if (g_signal == SIGINT)
					set_exit_status(shell, 130);
			}
			i++;
		}
		free_str_array(lines); // ✅ Lines freed
		lines = NULL; // ✅ Pointer nulled
	}

	clear_history();
	rl_clear_history();
	last_exit_status = get_exit_status(shell);
	free_shell_data(shell);
	return (last_exit_status);
}
