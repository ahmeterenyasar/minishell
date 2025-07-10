/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_processor_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 16:15:00 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/10 16:14:36 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_inside_quotes(const char *input, int pos)
{
	int	i;
	int	in_single_quotes;
	int	in_double_quotes;

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (i < pos)
	{
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (input[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (input[i] == '\\' && i + 1 < pos
			&& (in_double_quotes || (!in_single_quotes
					&& !in_double_quotes)))
			i++;
		i++;
	}
	return (in_single_quotes || in_double_quotes);
}

int	count_newlines(const char *converted)
{
	int	len;
	int	i;

	len = 1;
	i = 0;
	while (converted[i])
	{
		if (converted[i] == '\n' && !is_inside_quotes(converted, i))
			len++;
		i++;
	}
	return (len);
}

char	*create_command_segment(const char *converted, int start, int end)
{
	char	*segment;

	segment = malloc(end - start + 1);
	if (!segment)
		return (NULL);
	ft_strncpy(segment, converted + start, end - start);
	segment[end - start] = '\0';
	return (segment);
}

void	free_result_array(char **result, int count)
{
	while (count > 0)
		free(result[--count]);
	free(result);
}

char	**handle_empty_input(void)
{
	char	**commands;

	commands = malloc(sizeof(char *));
	if (commands)
		commands[0] = NULL;
	return (commands);
}
