#include "minishell.h"

extern sig_atomic_t	g_signal;

static int	handle_readline_result(char **input, t_shell_data *shell)
{
	if (!*input)
	{
		if (g_signal == SIGINT)
		{
			handle_sigint_in_readline(shell, input);
			g_signal = 0;  /* Reset signal after handling */
			return (1);
		}
		printf("exit\n");
		return (-1);
	}
	return (0);
}

static int	handle_post_readline_checks(char **input, t_shell_data *shell)
{
	if (g_signal == SIGINT)
	{
		handle_sigint_after_readline(shell, *input);
		g_signal = 0;  /* Reset signal after handling */
		return (1);
	}
	if (!**input)
	{
		free(*input);
		return (1);
	}
	return (0);
}

int	get_user_input(char **input, t_shell_data *shell)
{
	int	result;

	g_signal = 0;  /* Clear any previous signal */
	*input = readline("minishell$ ");
	result = handle_readline_result(input, shell);
	if (result != 0)
		return (result);
	result = handle_post_readline_checks(input, shell);
	return (result);
}
