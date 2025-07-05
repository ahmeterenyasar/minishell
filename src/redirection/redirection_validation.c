#include "minishell.h"

static int	check_ambiguous_redirect(t_redirect *redirect)
{
	if (!redirect->was_quoted && (ft_strchr(redirect->file, ' ')
			|| ft_strchr(redirect->file, '\t')))
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, redirect->file, ft_strlen(redirect->file));
		write(STDERR_FILENO, ": ambiguous redirect\n", 21);
		return (-1);
	}
	return (0);
}

int	validate_redirection_file(t_redirect *redirect)
{
	if (!redirect->file || !redirect->file[0])
	{
		write(STDERR_FILENO, "minishell: : No such file or directory\n", 39);
		return (-1);
	}
	return (check_ambiguous_redirect(redirect));
}
