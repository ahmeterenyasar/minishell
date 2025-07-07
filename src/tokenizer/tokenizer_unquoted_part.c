/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_unquoted_part.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:56:02 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:56:03 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	skip_escaped_char(const char *input, int *i)
{
	if (input[*i] == '\\' && input[*i + 1])
		*i += 2;
	else
		(*i)++;
}

static int	calculate_unquoted_length(const char *input, int *i)
{
	int	word_start;

	word_start = *i;
	while (input[*i] && !is_token_delimiter(input[*i])
		&& !is_operator_char(input[*i]) && !is_quote_char(input[*i]))
	{
		skip_escaped_char(input, i);
	}
	return (*i - word_start);
}

static int	prepare_unquoted_content(char **word_parts, int part_count,
		int word_len, int current_pos)
{
	if (!allocate_word_part(word_parts, part_count, word_len))
	{
		free_word_parts(word_parts, part_count);
		return (current_pos);
	}
	return (0);
}

int	process_unquoted_part(const char *input, int *i, char **word_parts,
		int *part_count, int *expandable)
{
	int	word_start;
	int	word_len;
	int	result;

	word_start = *i;
	word_len = calculate_unquoted_length(input, i);
	result = prepare_unquoted_content(word_parts, *part_count, word_len, *i);
	if (result != 0)
		return (result);
	copy_word_chars(input, word_start, word_parts[*part_count], word_len);
	*expandable = 1;
	finalize_word_part(part_count);
	return (0);
}
