/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_quoted_part.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:58 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 13:30:29 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_quote_error(char **word_parts, int part_count, int quote_end)
{
	free_word_parts(word_parts, part_count);
	if (quote_end == -1)
		return (-1);
	return (quote_end + 1);
}

static void	set_expandable_flag(char quote_char, int *expandable)
{
	if (quote_char == '"')
		*expandable = 1;
}

static int	prepare_quoted_content(char **word_parts, int part_count,
		int content_len, int quote_end)
{
	if (!allocate_word_part(word_parts, part_count, content_len))
		return (handle_quote_error(word_parts, part_count, quote_end));
	return (0);
}

int	process_quoted_part(const char *input, int *i, t_word_parts_params *params)
{
	char		quote_char;
	int			quote_start;
	int			quote_end;
	int			content_len;
	t_quote_pos	pos;

	quote_char = input[*i];
	quote_start = *i + 1;
	quote_end = find_closing_quote(input, *i, quote_char);
	if (quote_end == -1)
		return (handle_quote_error(params->word_parts, *params->part_count,
				quote_end));
	content_len = quote_end - quote_start;
	if (prepare_quoted_content(params->word_parts, *params->part_count,
			content_len, quote_end) != 0)
		return (quote_end + 1);
	pos.start = quote_start;
	pos.end = quote_end;
	pos.quote_char = quote_char;
	copy_quoted_text(input, pos, params->word_parts[*params->part_count]);
	set_expandable_flag(quote_char, params->expandable);
	finalize_word_part(params->part_count);
	*i = quote_end + 1;
	return (0);
}
