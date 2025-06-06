#include "minishell.h"

static int	setup_heredoc_reading(int *original_stdin)
{
	int	tty_fd;

	if (!isatty(STDIN_FILENO))
		write(2, "🚨 STDIN is not terminal!\n", 27);
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


static int	write_heredoc_line(int fd, char *line, int expand, t_shell_data *shell)
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


static void	cleanup_heredoc(char *clean_delimiter, int original_stdin)
{
	if (clean_delimiter)
		free(clean_delimiter);
	dup2(original_stdin, STDIN_FILENO);
	close(original_stdin);
	setup_signals(INTERACTIVE_MODE);
}

static int	read_heredoc_content(int fd, const char *delimiter, 
        int expand, t_shell_data *shell)
{
	char	*line;
	char	*clean_delimiter;
	int		original_stdin;
	int		interrupted;

	if (setup_heredoc_reading(&original_stdin))
		return (1);
	clean_delimiter = remove_quotes_from_delimiter((char *)delimiter);
	if (!clean_delimiter)
	{
		cleanup_heredoc(NULL, original_stdin);
		return (1);
	}

	interrupted = 0;
	while (1)
	{
		// Check for signal interruption
		if (g_signal == SIGINT)
		{
			interrupted = 1;
			break;
		}

		line = readline("heredoc> ");

		// readline returns NULL on EOF or signal interruption
		if (!line)
		{
			if (g_signal == SIGINT)
				interrupted = 1;
			else
				write(STDERR_FILENO, "\nminishell: warning: heredoc delimited by EOF\n", 45);
			break;
		}

		// Check if we got the delimiter
		if (ft_strcmp(line, clean_delimiter) == 0)
		{
			free(line);
			break;
		}

		// Write the line to heredoc file
		write_heredoc_line(fd, line, expand, shell);
		free(line);
	}

	cleanup_heredoc(clean_delimiter, original_stdin);
	return (interrupted ? 1 : 0);
}

int	process_heredoc(t_redirect *redirect, t_shell_data *shell)
{
	int		fd;
	char	*filename;
	int		result;
	int		expand;

	if (validate_heredoc_redirect(redirect))
		return (1);
	write(2, "minishell: processing heredoc for delimiter: ", 44);
	write(2, redirect->file, ft_strlen(redirect->file));
	write(2, "\n", 1);
	expand = !is_delimiter_quoted(redirect->file);
	fd = setup_heredoc_file(&filename);
	if (fd == -1)
	{
		write(2, "minishell: failed to setup heredoc file\n", 39);
		return (1);
	}
	result = read_heredoc_content(fd, redirect->file, expand, shell);
	close(fd);
	if (result)
	{
		unlink(filename);
		free(filename);
		return (1);
	}
	free(redirect->file);
	redirect->file = filename;
	write(2, "minishell: heredoc file created: ", 32);
	write(2, redirect->file, ft_strlen(redirect->file));
	write(2, "\n", 1);
	return (0);
}
