#include "minishell.h"

// CURRENTLY NOT WORKING PROPERLY

static char	*int_to_str(int n)
{
	char	*str;
	int		len;
	int		temp;
	int		i;

	temp = n;
	len = (n == 0) ? 1 : 0;
	while (temp > 0)
	{
		temp /= 10;
		len++;
	}
	str = malloc(len + 1);
	if (!str)
		return (NULL);
	str[len] = '\0';
	i = len - 1;
	if (n == 0)
		str[0] = '0';
	while (n > 0)
	{
		str[i--] = (n % 10) + '0';
		n /= 10;
	}
	return (str);
}

static char	*generate_heredoc_filename(void)
{
	static int	counter = 0;
	char		*filename;
	char		*pid_str;
	char		*counter_str;
	char		*temp;
	int			pid;

	pid = getpid();
	pid_str = int_to_str(pid);
	if (!pid_str)
		return (NULL);
	counter_str = int_to_str(counter++);
	if (!counter_str)
	{
		free(pid_str);
		return (NULL);
	}
	temp = ft_strjoin("/tmp/heredoc_", pid_str);
	free(pid_str);
	if (!temp)
	{
		free(counter_str);
		return (NULL);
	}
	filename = ft_strjoin(temp, "_");
	free(temp);
	if (!filename)
	{
		free(counter_str);
		return (NULL);
	}
	temp = ft_strjoin(filename, counter_str);
	free(filename);
	free(counter_str);
	return (temp);
}

static int	setup_heredoc_file(char **filename)
{
	int	fd;
	int	attempts;

	attempts = 0;
	while (attempts < 100)
	{
		*filename = generate_heredoc_filename();
		if (!*filename)
			return (-1);
		fd = open(*filename, O_CREAT | O_EXCL | O_RDWR, 0600);
		if (fd != -1)
			return (fd);
		free(*filename);
		*filename = NULL;
		attempts++;
	}
	return (-1);
}

static int	handle_heredoc_interrupt(char *line, int original_stdin)
{
	if (line)
		free(line);
	if (g_signal == SIGINT)
	{
		dup2(original_stdin, STDIN_FILENO);
		close(original_stdin);
		setup_signals(INTERACTIVE_MODE);
		return (1);
	}
	return (0);
}

static int	process_heredoc_line(char *line, const char *delimiter, int fd,
		t_shell_data *shell, int expand)
{
	char	*expanded_line;

	if (ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	if (expand)
	{
		expanded_line = expand_env_vars(line, shell);
		free(line);
		line = expanded_line;
	}
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
	free(line);
	return (0);
}

static int	setup_heredoc_reading(int *original_stdin)
{
	*original_stdin = dup(STDIN_FILENO);
	if (*original_stdin == -1)
		return (1);
	setup_signals(HEREDOC_MODE);
	g_signal = 0;
	return (0);
}

static int	read_heredoc_content(int fd, const char *delimiter, int expand,
		t_shell_data *shell)
{
	char	*line;
	int		original_stdin;
	int		delimiter_found;

	if (setup_heredoc_reading(&original_stdin))
		return (1);
	delimiter_found = 0;
	while (!delimiter_found)
	{
		line = readline("> ");
		if (!line || g_signal == SIGINT)
		{
			if (handle_heredoc_interrupt(line, original_stdin))
				return (1);
			break ;
		}
		delimiter_found = process_heredoc_line(line, delimiter, fd, shell,
				expand);
	}
	setup_signals(INTERACTIVE_MODE);
	close(original_stdin);
	return (0);
}

static int	cleanup_heredoc_failure(char *filename)
{
	unlink(filename);
	free(filename);
	return (1);
}

int	process_heredoc(t_redirect *redirect, t_shell_data *shell)
{
	int		fd;
	char	*filename;
	int		result;

	if (!redirect || redirect->type != TOKEN_HEREDOC)
		return (1);
	fd = setup_heredoc_file(&filename);
	if (fd == -1)
		return (1);
	result = read_heredoc_content(fd, redirect->file, redirect->expand_heredoc,
			shell);
	close(fd);
	if (result)
		return (cleanup_heredoc_failure(filename));
	free(redirect->file);
	redirect->file = filename;
	return (0);
}

static int	process_cmd_heredocs(t_command *cmd, t_shell_data *shell)
{
	t_redirect	*curr_redir;

	curr_redir = cmd->redirects;
	while (curr_redir)
	{
		if (curr_redir->type == TOKEN_HEREDOC)
		{
			if (process_heredoc(curr_redir, shell))
				return (1);
		}
		curr_redir = curr_redir->next;
	}
	return (0);
}

int	process_all_heredocs(t_command *cmd, t_shell_data *shell)
{
	t_command	*curr_cmd;

	curr_cmd = cmd;
	while (curr_cmd)
	{
		if (process_cmd_heredocs(curr_cmd, shell))
			return (1);
		curr_cmd = curr_cmd->next;
	}
	return (0);
}
