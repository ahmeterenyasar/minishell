/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_file_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:54:33 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/11 21:07:25 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_counter_strings(char **counter_str, char **session_str)
{
	static int	counter = 0;
	static int	session_id = 0;

	if (session_id == 0)
		session_id = (int)((unsigned long)&counter % 99999) + 1;
	*session_str = int_to_string(session_id);
	if (!*session_str)
		return (NULL);
	*counter_str = int_to_string(counter++);
	if (!*counter_str)
	{
		free(*session_str);
		return (NULL);
	}
	return (*session_str);
}

static char	*build_filename_path(char *session_str, char *counter_str)
{
	char	*temp;
	char	*filename;
	char	*result;

	temp = ft_strjoin("/tmp/heredoc_", session_str);
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
	char	*session_str;
	char	*counter_str;
	char	*filename;

	if (!create_counter_strings(&counter_str, &session_str))
		return (NULL);
	filename = build_filename_path(session_str, counter_str);
	free(session_str);
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
