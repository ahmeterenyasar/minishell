/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:43 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/10 16:05:06 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	skip_quoted_content(const char *input, int *i, char quote_char)
{
	while (input[*i] && input[*i] != quote_char)
	{
		if (quote_char == '"' && input[*i] == '\\' && input[*i + 1])
			*i += 2;
		else
			(*i)++;
	}
	return (input[*i] == quote_char);
}

static int	process_quote_pair(const char *input, int *i)
{
	char	quote_char;
	int		found_closing;

	quote_char = input[*i];
	(*i)++;
	found_closing = skip_quoted_content(input, i, quote_char);
	if (!found_closing)
		return (1);
	(*i)++;
	return (0);
}

int	has_unclosed_quotes(const char *input)
{
	int	i;

	if (!input)
		return (0);
	i = 0;
	while (input[i])
	{
		if (is_quote_char(input[i]))
		{
			if (process_quote_pair(input, &i))
				return (1);
		}
		else
			i++;
	}
	return (0);
}

int	skip_unquoted_text(const char *input, int pos)
{
	while (input[pos] && !is_token_delimiter(input[pos])
		&& !is_operator_char(input[pos]) && !is_quote_char(input[pos]))
	{
		if (input[pos] == '\\' && input[pos + 1])
			pos += 2;
		else
			pos++;
	}
	return (pos);
}
