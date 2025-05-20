#include "minishell.h"
#include <stdio.h>

sig_atomic_t	g_signal = 0;

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_command	*cmd;

	(void)argc;
	(void)argv;
	while (1)
	{
		g_signal = 0;
		input = readline("miniyarrak$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		cmd = parser_interface(input, envp);
		// debug: print the command structure
		if (cmd)
		{
			// print_command(cmd);
			// free_command(cmd);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
