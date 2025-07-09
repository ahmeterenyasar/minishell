/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:56:07 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 13:41:03 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_word_parts_loop(const char *input, int *i,
		t_word_parts_params *params)
{
	int	result;

	while (input[*i] && !is_token_delimiter(input[*i])
		&& !is_operator_char(input[*i]))
	{
		if (is_quote_char(input[*i]))
			result = process_quoted_part(input, i, params);
		else
			result = process_unquoted_part(input, i, params);
		if (result != 0)
			return (result);
	}
	return (0);
}

static t_finalize_params	init_finalize_params(const char *input,
		int start_pos, int current_pos, int expandable)
{
	t_finalize_params	params;

	params.start_pos = start_pos;
	params.expandable = expandable;
	params.input = input;
	params.current_pos = current_pos;
	return (params);
}

int	process_concatenated_word(const char *input, int i, t_token **head)
{
	char				*word_parts[256];
	int					part_count;
	int					expandable;
	int					start_pos;
	t_word_parts_params	params;

	part_count = 0;
	expandable = 0;
	start_pos = i;
	params.word_parts = word_parts;
	params.part_count = &part_count;
	params.expandable = &expandable;
	if (process_word_parts_loop(input, &i, &params) != 0)
		return (i);
	return (finalize_concatenated_word(word_parts, part_count,
			init_finalize_params(input, start_pos, i, expandable), head));
}

t_token	*tokenize(const char *input)
{
	t_token	*head;
	int		i;
	int		result;

	if (!input)
		return (NULL);
	head = NULL;
	i = 0;
	while (input[i])
	{
		i = skip_white_space(input, i);
		if (!input[i])
			break ;
		result = process_single_token(input, i, &head);
		if (result == -1)
		{
			free_tokens(head);
			return (NULL);
		}
		i = result;
	}
	return (head);
}
