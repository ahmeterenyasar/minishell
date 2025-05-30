#include "minishell.h"

char	*remove_quotes_from_delimiter(char *delimiter)
{
	int		len;
	char	*result;
	int		i, j;

	if (!delimiter)
		return (NULL);
	len = ft_strlen(delimiter);
	if (len >= 2 && ((delimiter[0] == '"' && delimiter[len - 1] == '"') ||
		(delimiter[0] == '\'' && delimiter[len - 1] == '\'')))
	{
		result = malloc(len - 1);
		if (!result)
			return (NULL);
		i = 1;
		j = 0;
		while (i < len - 1)
			result[j++] = delimiter[i++];
		result[j] = '\0';
		return (result);
	}
	return (ft_strdup(delimiter));
}

int	is_delimiter_quoted(char *delimiter)
{
	int	len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len >= 2 && ((delimiter[0] == '"' && delimiter[len - 1] == '"') ||
		(delimiter[0] == '\'' && delimiter[len - 1] == '\'')))
		return (1);
	return (0);
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
	pid_str = int_to_string(pid);
	if (!pid_str)
		return (NULL);
	counter_str = int_to_string(counter++);
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

int	setup_heredoc_file(char **filename)
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

int	validate_heredoc_redirect(t_redirect *redirect)
{
	if (!redirect || redirect->type != TOKEN_HEREDOC)
	{
		write(2, "minishell: invalid heredoc redirect\n", 35);
		return (1);
	}
	return (0);
}