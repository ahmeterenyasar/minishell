#include "minishell.h"

void	print_file_error(const char *filename, const char *operation)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, filename, ft_strlen(filename));
	write(STDERR_FILENO, ": ", 2);
	perror("");
	(void)operation;
}

int	check_file_permissions(const char *filename, int flags)
{
	struct stat	file_stat;

	if (stat(filename, &file_stat) == -1)
	{
		if (errno == ENOENT && (flags & O_CREAT))
			return (0); // File doesn't exist but will be created
		return (-1);
	}
	
	if (S_ISDIR(file_stat.st_mode))
	{
		errno = EISDIR;
		return (-1);
	}
	
	if (flags & O_RDONLY)
	{
		if (access(filename, R_OK) == -1)
			return (-1);
	}
	else if (flags & (O_WRONLY | O_APPEND))
	{
		if (access(filename, W_OK) == -1 && errno != ENOENT)
			return (-1);
	}
	
	return (0);
}
