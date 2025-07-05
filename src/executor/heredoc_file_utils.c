#include "minishell.h"

static char	*create_pid_counter_strings(char **pid_str, char **counter_str)
{
	static int	counter;
	int			pid;

	counter = 0;
	pid = getpid();
	*pid_str = int_to_string(pid);
	if (!*pid_str)
		return (NULL);
	*counter_str = int_to_string(counter++);
	if (!*counter_str)
	{
		free(*pid_str);
		return (NULL);
	}
	return (*pid_str);
}

static char	*build_filename_path(char *pid_str, char *counter_str)
{
	char	*temp;
	char	*filename;
	char	*result;

	temp = ft_strjoin("/tmp/heredoc_", pid_str);
	if (!temp)
		return (NULL);
	filename = ft_strjoin(temp, "_");
	free(temp);
	if (!filename)
		return (NULL);
	result = ft_strjoin(filename, counter_str);
	free(filename);
	return (result);
}

static char	*generate_heredoc_filename(void)
{
	char	*pid_str;
	char	*counter_str;
	char	*filename;

	if (!create_pid_counter_strings(&pid_str, &counter_str))
		return (NULL);
	filename = build_filename_path(pid_str, counter_str);
	free(pid_str);
	free(counter_str);
	return (filename);
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
