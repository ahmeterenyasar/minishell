/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_line_processing.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:54:35 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 14:40:32 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_signal_interruption(char *line, int original_stdin,
		char **clean_delimiter)
{
	if (g_signal == SIGINT)
	{
		if (line)
			free(line);
		cleanup_heredoc_reading(original_stdin, clean_delimiter);
		return (1);
	}
	return (0);
}

int	handle_eof_condition(char *line)
{
	if (!line)
	{
		write(STDERR_FILENO,
			"\nminishell: warning: heredoc delimited by EOF\n", 45);
		return (1);
	}
	return (0);
}

int	check_delimiter_match(char *line, char *clean_delimiter)
{
	if (ft_strcmp(line, clean_delimiter) == 0)
	{
		free(line);
		return (1);
	}
	return (0);
}

int	process_heredoc_line(int fd, char *line, int expand, t_shell_data *shell)
{
	write_heredoc_line(fd, line, expand, shell);
	free(line);
	return (0);
}
