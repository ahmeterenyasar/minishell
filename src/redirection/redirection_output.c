/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_output.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:54:53 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:54:54 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_output_flags(t_redirect *redirect)
{
	if (redirect->type == TOKEN_REDIR_OUT)
		return (O_WRONLY | O_CREAT | O_TRUNC);
	else if (redirect->type == TOKEN_APPEND)
		return (O_WRONLY | O_CREAT | O_APPEND);
	return (-1);
}

static int	open_and_dup_output_file(char *file, int flags)
{
	int	fd;

	fd = open(file, flags, 0644);
	if (fd == -1)
	{
		print_file_error(file, "output");
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

int	setup_output_redirection(t_redirect *redirect)
{
	int	flags;

	flags = get_output_flags(redirect);
	if (flags == -1)
		return (-1);
	if (check_file_permissions(redirect->file, flags) == -1)
	{
		print_file_error(redirect->file, "output");
		return (-1);
	}
	return (open_and_dup_output_file(redirect->file, flags));
}
