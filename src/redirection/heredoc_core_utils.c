#include "minishell.h"

int	setup_heredoc_reading(int *original_stdin)
{
	int	tty_fd;

	if (!isatty(STDIN_FILENO))
	{
		/* STDIN is not a terminal - this is expected in some cases */
	}
	*original_stdin = dup(STDIN_FILENO);
	if (*original_stdin == -1)
		return (1);
	tty_fd = open("/dev/tty", O_RDONLY);
	if (tty_fd == -1)
		return (1);
	if (dup2(tty_fd, STDIN_FILENO) == -1)
	{
		close(tty_fd);
		return (1);
	}
	close(tty_fd);
	g_signal = 0;
	setup_signals(HEREDOC_MODE);
	return (0);
}

int	write_heredoc_line(int fd, char *line, int expand, t_shell_data *shell)
{
	char	*expanded_line;

	if (expand)
	{
		expanded_line = expand_env_vars(line, shell);
		if (expanded_line)
		{
			write(fd, expanded_line, ft_strlen(expanded_line));
			free(expanded_line);
		}
		else
			write(fd, line, ft_strlen(line));
	}
	else
		write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	return (0);
}

void	cleanup_heredoc_reading(int original_stdin, char **clean_delimiter)
{
	if (clean_delimiter && *clean_delimiter)
	{
		free(*clean_delimiter);
		*clean_delimiter = NULL;
	}
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	setup_signals(INTERACTIVE_MODE);
}
