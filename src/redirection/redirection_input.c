/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_input.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:54:50 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:54:51 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_file_input_redirection(t_redirect *redirect)
{
	int	fd;

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
	return (0);
}

static int	handle_heredoc_redirection(t_redirect *redirect)
{
	int	fd;

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
	return (0);
}

int	setup_input_redirection(t_redirect *redirect)
{
	if (redirect->type == TOKEN_REDIR_IN)
		return (handle_file_input_redirection(redirect));
	else if (redirect->type == TOKEN_HEREDOC)
		return (handle_heredoc_redirection(redirect));
	return (0);
}
