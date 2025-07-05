#include "minishell.h"

extern sig_atomic_t	g_signal;

static int	execute_parsed_command(t_command *cmd, t_shell_data *shell)
{
	setup_signals(EXECUTING_MODE);
	execute_command(cmd, shell);
	setup_signals(INTERACTIVE_MODE);
	if (shell->should_exit)
	{
		free_command(cmd);
		clear_current_lines(shell);
		free_shell_data(shell);
	}
	return (0);
}

static int	handle_parsing_failure(t_shell_data *shell)
{
	if (g_signal == SIGINT)
		handle_sigint_in_parsing(shell);
	return (1);
}

int	process_single_command(char *line, t_shell_data *shell)
{
	t_command	*cmd;

	cmd = parse_input(line, shell);
	if (cmd)
	{
		execute_parsed_command(cmd, shell);
		free_command(cmd);
		return (0);
	}
	else
	{
		return (handle_parsing_failure(shell));
	}
}

void	process_multiple_commands(char **lines, t_shell_data *shell)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		if (*lines[i])
			process_single_command(lines[i], shell);
		i++;
	}
}
