/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_processing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:38 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 19:07:58 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_quote_segments(const char *input, int start_pos,
		int *has_unquoted)
{
	int		pos;
	int		quote_segments;
	char	quote_char;

	pos = start_pos;
	quote_segments = 0;
	while (input[pos] && !is_token_delimiter(input[pos])
		&& !is_operator_char(input[pos]))
	{
		if (is_quote_char(input[pos]))
		{
			quote_segments++;
			quote_char = input[pos];
			pos = find_closing_quote(input, pos, quote_char);
			if (pos == -1)
				return (-1);
			pos++;
		}
		else
		{
			*has_unquoted = 1;
			pos = skip_unquoted_text(input, pos);
		}
	}
	return (quote_segments);
}

int	should_use_concatenation(const char *input, int i)
{
	int	quote_segments;
	int	has_unquoted;

	has_unquoted = 0;
	quote_segments = count_quote_segments(input, i, &has_unquoted);
	if (quote_segments == -1)
		return (0);
	return (quote_segments > 1 || (quote_segments > 0 && has_unquoted));
}

static int	handle_quote_processing(const char *input, int i, t_token **head)
{
	int	result;

	result = handle_quotes(input, i, head);
	if (result == -1)
	{
		free_tokens(*head);
		return (-1);
	}
	return (result);
}

int	process_token(const char *input, int i, t_token **head)
{
	if (should_use_concatenation(input, i))
		return (process_concatenated_word(input, i, head));
	if (input[i] == '\'' || input[i] == '"')
		return (handle_quote_processing(input, i, head));
	if (input[i] == '|')
		return (handle_pipe(input, i, head));
	if (input[i] == '<')
		return (handle_redir_in(input, i, head));
	if (input[i] == '>')
		return (handle_redir_out(input, i, head));
	return (add_word_token(input, i, head));
}

int	process_single_token(const char *processed_input, int i, t_token **head)
{
	int	result;

	result = process_token(processed_input, i, head);
	if (result == -1)
		return (-1);
	return (result);
}
