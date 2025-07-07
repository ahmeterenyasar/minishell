/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_setup.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:54:43 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 14:31:51 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	prepare_heredoc_reading(const char *delimiter, int *original_stdin,
		char **clean_delimiter)
{
	if (setup_heredoc_reading(original_stdin))
		return (1);
	*clean_delimiter = remove_quotes_from_delimiter((char *)delimiter);
	if (!*clean_delimiter)
	{
		cleanup_heredoc_reading(*original_stdin, NULL);
		return (1);
	}
	return (0);
}

int	read_heredoc_content(int fd, const char *delimiter, int expand,
		t_shell_data *shell)
{
	char				*clean_delimiter;
	int					original_stdin;
	int					result;
	t_heredoc_context	ctx;

	if (prepare_heredoc_reading(delimiter, &original_stdin, &clean_delimiter))
		return (1);
	ctx.original_stdin = original_stdin;
	ctx.clean_delimiter = &clean_delimiter;
	ctx.expand = expand;
	ctx.shell = shell;
	result = read_heredoc_loop(fd, delimiter, &ctx);
	cleanup_heredoc_reading(original_stdin, &clean_delimiter);
	return (result);
}

int	handle_heredoc_failure(char *filename, int result)
{
	if (result)
	{
		unlink(filename);
		free(filename);
		return (1);
	}
	return (0);
}

void	finalize_heredoc_success(t_redirect *redirect, char *filename)
{
	free(redirect->file);
	redirect->file = filename;
}
