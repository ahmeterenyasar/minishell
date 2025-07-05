#include "minishell.h"

static int	check_signal_before_read(int original_stdin, char **clean_delimiter)
{
	if (g_signal == SIGINT)
	{
		cleanup_heredoc_reading(original_stdin, clean_delimiter);
		return (1);
	}
	return (0);
}

static int	process_single_heredoc_line(int fd, int expand, t_shell_data *shell,
		int original_stdin, char **clean_delimiter)
{
	char	*line;

	if (check_signal_before_read(original_stdin, clean_delimiter))
		return (1);
	line = readline("heredoc> ");
	if (check_signal_interruption(line, original_stdin, clean_delimiter))
		return (1);
	if (handle_eof_condition(line))
		return (2);
	if (check_delimiter_match(line, *clean_delimiter))
		return (2);
	process_heredoc_line(fd, line, expand, shell);
	return (0);
}

int	read_heredoc_loop(int fd, const char *delimiter, int expand,
		t_shell_data *shell, int original_stdin, char **clean_delimiter)
{
	int	result;
	int	continue_reading;

	(void)delimiter;
	continue_reading = 1;
	while (continue_reading)
	{
		result = process_single_heredoc_line(fd, expand, shell, original_stdin,
				clean_delimiter);
		if (result == 1)
			return (1);
		if (result == 2)
			continue_reading = 0;
	}
	return (0);
}
