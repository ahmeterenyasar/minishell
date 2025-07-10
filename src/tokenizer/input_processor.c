/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:30 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/10 16:16:13 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*convert_newlines(const char *input)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '\\' && input[i + 1] == 'n')
		{
			result[j++] = '\n';
			i += 2;
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

static int	fill_command_segment(const char *converted, char **result,
		int start, int j)
{
	int	end;

	end = start;
	while (converted[end] && (converted[end] != '\n'
			|| is_inside_quotes(converted, end)))
		end++;
	if (end > start || j == 0)
	{
		result[j] = create_command_segment(converted, start, end);
		if (!result[j])
			return (-1);
	}
	return (end + 1);
}

static char	**process_command_segments(const char *converted, int len)
{
	char	**result;
	int		i;
	int		j;

	result = malloc(sizeof(char *) * (len + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (j < len)
	{
		i = fill_command_segment(converted, result, i, j);
		if (i == -1)
		{
			free_result_array(result, j);
			return (NULL);
		}
		j++;
	}
	result[j] = NULL;
	return (result);
}

char	**split_commands_by_newlines(const char *input)
{
	char	*converted;
	char	**result;
	int		len;

	if (!input || !*input)
		return (handle_empty_input());
	converted = convert_newlines(input);
	if (!converted)
		return (NULL);
	len = count_newlines(converted);
	result = process_command_segments(converted, len);
	free(converted);
	return (result);
}
