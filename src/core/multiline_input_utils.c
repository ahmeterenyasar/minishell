/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiline_input_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 18:00:00 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/10 16:44:26 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern sig_atomic_t	g_signal;

static char	*append_line_to_result(char *result, char *line)
{
	char	*temp;
	char	*new_result;

	temp = result;
	new_result = ft_strjoin(result, "\n");
	free(temp);
	if (!new_result)
	{
		free(line);
		return (NULL);
	}
	temp = new_result;
	new_result = ft_strjoin(new_result, line);
	free(temp);
	free(line);
	return (new_result);
}

static char	*handle_multiline_continuation(char *result)
{
	char	*line;
	char	*new_result;

	line = readline("> ");
	if (!line || g_signal == SIGINT)
	{
		free(result);
		if (line)
			free(line);
		return (NULL);
	}
	new_result = append_line_to_result(result, line);
	return (new_result);
}

char	*read_multiline_input(char *initial_input, t_shell_data *shell)
{
	char	*result;

	(void)shell;
	result = ft_strdup(initial_input);
	if (!result)
		return (NULL);
	while (has_unclosed_quotes(result))
	{
		result = handle_multiline_continuation(result);
		if (!result)
			return (NULL);
	}
	return (result);
}
