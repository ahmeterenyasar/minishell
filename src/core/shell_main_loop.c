#include "minishell.h"

extern sig_atomic_t	g_signal;

static int	process_input_line(char *input, t_shell_data *shell)
{
	char	**lines;

	add_history(input);
	lines = split_commands_by_newlines(input);
	free(input);
	if (!lines)
		return (1);
	set_current_lines(shell, lines);
	process_multiple_commands(lines, shell);
	clear_current_lines(shell);
	if (shell->should_exit)
		return (-1);
	return (0);
}

static int	handle_main_loop_iteration(t_shell_data *shell)
{
	char	*input;
	int		input_result;

	input_result = get_user_input(&input, shell);
	if (input_result == -1)
		return (-1);
	if (input_result == 1)
	{
		/* Signal occurred but no command was processed */
		/* Signal already handled in input handler, reset signals for next iteration */
		setup_signals(INTERACTIVE_MODE);
		return (1);
	}
	input_result = process_input_line(input, shell);
	if (input_result == -1)
		return (-1);
	/* Don't call handle_signal_check here - commands handle their own signals */
	setup_signals(INTERACTIVE_MODE);
	return (0);
}

void	run_shell_main_loop(t_shell_data *shell)
{
	int	loop_result;

	while (1)
	{
		loop_result = handle_main_loop_iteration(shell);
		if (loop_result == -1)
			break ;
	}
}
