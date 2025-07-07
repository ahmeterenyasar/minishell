/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:56:07 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:56:08 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	process_concatenated_word(const char *input, int i, t_token **head)
{
	char	*word_parts[256];
	int		part_count;
	int		start_pos;
	int		expandable;
	int		result;

	part_count = 0;
	expandable = 0;
	start_pos = i;
	while (input[i] && !is_token_delimiter(input[i])
		&& !is_operator_char(input[i]))
	{
		if (is_quote_char(input[i]))
			result = process_quoted_part(input, &i, word_parts, &part_count,
					&expandable);
		else
			result = process_unquoted_part(input, &i, word_parts, &part_count,
					&expandable);
		if (result != 0)
			return (result);
	}
	return (finalize_concatenated_word(word_parts, part_count, start_pos,
			expandable, head, input, i));
}

t_token	*tokenize(const char *input)
{
	t_token	*head;
	int		i;
	int		result;
	char	*processed_input;

	processed_input = handle_newlines(input);
	if (!processed_input)
		return (NULL);
	head = NULL;
	i = 0;
	while (processed_input[i])
	{
		i = skip_white_space(processed_input, i);
		if (!processed_input[i])
			break ;
		result = process_token(processed_input, i, &head);
		if (result == -1)
		{
			free(processed_input);
			return (NULL);
		}
		i = result;
	}
	free(processed_input);
	return (head);
}
