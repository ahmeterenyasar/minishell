#include "minishell.h"

static void	print_file_error(const char *filename, const char *operation)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, filename, ft_strlen(filename));
	write(STDERR_FILENO, ": ", 2);
	perror("");
	(void)operation;
}

static int	check_file_permissions(const char *filename, int flags)
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

int setup_input_redirection(t_redirect *redirect)
{
    int fd;

    if (redirect->type == TOKEN_REDIR_IN)
    {
        if (check_file_permissions(redirect->file, O_RDONLY) == -1)
        {
            print_file_error(redirect->file, "input");
            return (-1);
        }
        
        fd = open(redirect->file, O_RDONLY);
        if (fd == -1)
        {
            print_file_error(redirect->file, "input");
            return (-1);
        }
        
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            return (-1);
        }
        close(fd);
    }
    else if (redirect->type == TOKEN_HEREDOC)
    {
        fd = open(redirect->file, O_RDONLY);
        if (fd == -1)
        {
            print_file_error(redirect->file, "heredoc");
            return (-1);
        }
        
        if (dup2(fd, STDIN_FILENO) == -1)
        {
            perror("dup2");
            close(fd);
            return (-1);
        }
        close(fd);
        // Do NOT unlink here; let the executor or cleanup handle it
    }
    return (0);
}

int	setup_output_redirection(t_redirect *redirect)
{
	int	fd;
	int	flags;

	if (redirect->type == TOKEN_REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (redirect->type == TOKEN_APPEND)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		return (-1);

	if (check_file_permissions(redirect->file, flags) == -1)
	{
		print_file_error(redirect->file, "output");
		return (-1);
	}

	fd = open(redirect->file, flags, 0644);
	if (fd == -1)
	{
		print_file_error(redirect->file, "output");
		return (-1);
	}
	
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	validate_redirection_file(t_redirect *redirect)
{
	if (!redirect->file || !redirect->file[0])
	{
		write(STDERR_FILENO, "minishell: : No such file or directory\n", 39);
		return (-1);
	}
	
	// Check for ambiguous redirects (multiple words after expansion)
	if (ft_strchr(redirect->file, ' ') || ft_strchr(redirect->file, '\t'))
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, redirect->file, ft_strlen(redirect->file));
		write(STDERR_FILENO, ": ambiguous redirect\n", 21);
		return (-1);
	}
	
	return (0);
}

int	setup_redirections(t_redirect *redirects)
{
	t_redirect	*current;

	current = redirects;
	while (current)
	{
		if (validate_redirection_file(current) == -1)
			return (-1);
			
		if (current->type == TOKEN_REDIR_IN || current->type == TOKEN_HEREDOC)
		{
			if (setup_input_redirection(current) == -1)
				return (-1);
		}
		else if (current->type == TOKEN_REDIR_OUT || current->type == TOKEN_APPEND)
		{
			if (setup_output_redirection(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}