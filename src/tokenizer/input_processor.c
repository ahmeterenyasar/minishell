/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_processor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:30 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:55:31 by ayasar           ###   ########.fr       */
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

static int	is_inside_quotes(const char *input, int pos)
{
	int		i;
	int		in_single_quotes;
	int		in_double_quotes;

	i = 0;
	in_single_quotes = 0;
	in_double_quotes = 0;
	while (i < pos)
	{
		if (input[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (input[i] == '"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		else if (input[i] == '\\' && i + 1 < pos && 
				 (in_double_quotes || (!in_single_quotes && !in_double_quotes)))
			i++;
		i++;
	}
	return (in_single_quotes || in_double_quotes);
}

char	**split_commands_by_newlines(const char *input)
{
	char	*converted;
	char	**commands;
	char	**result;
	int		i;
	int		j;
	int		len;

	if (!input || !*input)
	{
		commands = malloc(sizeof(char *));
		if (commands)
			commands[0] = NULL;
		return (commands);
	}
	converted = convert_newlines(input);
	if (!converted)
		return (NULL);
	
	/* Count newlines that are NOT inside quotes */
	len = 1;
	i = 0;
	while (converted[i])
	{
		if (converted[i] == '\n' && !is_inside_quotes(converted, i))
			len++;
		i++;
	}
	
	/* Allocate result array */
	result = malloc(sizeof(char *) * (len + 1));
	if (!result)
	{
		free(converted);
		return (NULL);
	}
	
	/* Split only on newlines that are NOT inside quotes */
	j = 0;
	i = 0;
	while (j < len)
	{
		int start = i;
		int end = i;
		
		/* Find the end of current command */
		while (converted[end] && (converted[end] != '\n' || is_inside_quotes(converted, end)))
			end++;
		
		/* Copy the command */
		if (end > start || j == 0)
		{
			result[j] = malloc(end - start + 1);
			if (!result[j])
			{
				/* Free allocated memory on error */
				while (j > 0)
					free(result[--j]);
				free(result);
				free(converted);
				return (NULL);
			}
			ft_strncpy(result[j], converted + start, end - start);
			result[j][end - start] = '\0';
			j++;
		}
		i = end + 1;
	}
	result[j] = NULL;
	free(converted);
	return (result);
}
