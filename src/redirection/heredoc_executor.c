#include "minishell.h"

static int	setup_heredoc_file_and_process(t_redirect *redirect,
											t_shell_data *shell,
											char **filename)
{
	int	fd;
	int	result;

	fd = setup_heredoc_file(filename);
	if (fd == -1)
	{
		write(2, "minishell: failed to setup heredoc file\n", 41);
		return (-1);
	}
	result = read_heredoc_content(fd, redirect->file, redirect->expand_heredoc,
			shell);
	close(fd);
	return (result);
}

int	process_heredoc(t_redirect *redirect, t_shell_data *shell)
{
	char	*filename;
	int		result;

	if (validate_heredoc_redirect(redirect))
		return (1);
	write(2, "minishell: processing heredoc for delimiter: ", 46);
	write(2, redirect->file, ft_strlen(redirect->file));
	write(2, "\n", 1);
	result = setup_heredoc_file_and_process(redirect, shell, &filename);
	if (result == -1)
	{
		if (filename)
			free(filename);
		return (1);
	}
	if (handle_heredoc_failure(filename, result))
		return (1);
	finalize_heredoc_success(redirect, filename);
	return (0);
}
