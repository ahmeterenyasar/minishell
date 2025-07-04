#include "minishell.h"

sig_atomic_t	g_signal = 0;

static void	handle_signal_check(t_shell_data *shell)
{
	if (g_signal == SIGINT)
		set_exit_status(shell, 130);
	g_signal = 0;
}

static int	handle_readline_input(char **input, t_shell_data *shell)
{
	*input = readline("minishell$ ");
	if (g_signal == SIGINT)
	{
		set_exit_status(shell, 130);
		if (*input)
		{
			free(*input);
			*input = NULL;
		}
		return (1);
	}
	if (!*input)
	{
		printf("exit\n");
		return (-1);
	}
	if (g_signal == SIGINT)
	{
		set_exit_status(shell, 130);
		free(*input);
		return (1);
	}
	if (!**input)
	{
		free(*input);
		return (1);
	}
	return (0);
}

static int	process_command_line(char *line, t_shell_data *shell)
{
	t_command	*cmd;

	cmd = parse_input(line, shell);
	if (cmd)
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
		free_command(cmd);
		return (0);
	}
	else
	{
		if (g_signal == SIGINT)
		{
			set_exit_status(shell, 130);
			setup_signals(INTERACTIVE_MODE);
		}
		return (1);
	}
}

static void	process_command_lines(char **lines, t_shell_data *shell)
{
	int	i;

	i = 0;
	while (lines[i])
	{
		if (*lines[i])
			process_command_line(lines[i], shell);
		i++;
	}
}

int	main(int argc, char **argv, char **envp)
{
	char			*input;
	t_shell_data	*shell;
	char			**lines;
	int				input_result;
	(void)argc;
	(void)argv;

	shell = init_shell_data(envp);
	if (!shell)
		return (1);
	setup_signals(INTERACTIVE_MODE);
	while (1)
	{
		handle_signal_check(shell);
		input_result = handle_readline_input(&input, shell);
		if (input_result == -1)
			break ;
		if (input_result == 1)
			continue ;
		add_history(input);
		lines = split_commands_by_newlines(input);
		free(input);
		if (!lines)
			continue ;
		set_current_lines(shell, lines);
		process_command_lines(lines, shell);
		clear_current_lines(shell);
		handle_signal_check(shell);
		setup_signals(INTERACTIVE_MODE);
	}
	free_shell_data(shell);
	return (0);
}
