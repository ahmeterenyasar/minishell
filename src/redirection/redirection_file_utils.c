/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_file_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:54:48 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:54:49 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_file_error(const char *filename, const char *operation)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, filename, ft_strlen(filename));
	write(STDERR_FILENO, ": ", 2);
	perror("");
	(void)operation;
}

static int	check_file_stat(const char *filename, int flags)
{
	struct stat	file_stat;

	if (stat(filename, &file_stat) == -1)
	{
		if (errno == ENOENT && (flags & O_CREAT))
			return (0);
		return (-1);
	}
	if (S_ISDIR(file_stat.st_mode))
	{
		errno = EISDIR;
		return (-1);
	}
	return (0);
}

static int	check_read_permissions(const char *filename)
{
	if (access(filename, R_OK) == -1)
		return (-1);
	return (0);
}

static int	check_write_permissions(const char *filename)
{
	if (access(filename, W_OK) == -1 && errno != ENOENT)
		return (-1);
	return (0);
}

int	check_file_permissions(const char *filename, int flags)
{
	if (check_file_stat(filename, flags) == -1)
		return (-1);
	if (flags & O_RDONLY)
	{
		if (check_read_permissions(filename) == -1)
			return (-1);
	}
	else if (flags & (O_WRONLY | O_APPEND))
	{
		if (check_write_permissions(filename) == -1)
			return (-1);
	}
	return (0);
}
