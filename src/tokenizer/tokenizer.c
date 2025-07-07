/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:56:07 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 13:20:39 by ayasar           ###   ########.fr       */
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
			result = process_quoted_part(input, i, params->word_parts,
					params->part_count, params->expandable);
		else
			result = process_unquoted_part(input, i, params->word_parts,
					params->part_count, params->expandable);
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

	part_count = 0;
	expandable = 0;
	start_pos = i;
	if (process_word_parts_loop(input, &i, &(t_word_parts_params){
			word_parts, &part_count, &expandable}) != 0)
		return (i);
	return (finalize_concatenated_word(word_parts, part_count,
			init_finalize_params(input, start_pos, i, expandable), head));
}

static t_token	*process_input_tokens(char *processed_input)
{
	t_token	*head;
	int		i;
	int		result;

	head = NULL;
	i = 0;
	while (processed_input[i])
	{
		i = skip_white_space(processed_input, i);
		if (!processed_input[i])
			break ;
		result = process_single_token(processed_input, i, &head);
		if (result == -1)
			return (NULL);
		i = result;
	}
	return (head);
}

t_token	*tokenize(const char *input)
{
	t_token	*head;
	char	*processed_input;

	processed_input = handle_newlines(input);
	if (!processed_input)
		return (NULL);
	head = process_input_tokens(processed_input);
	free(processed_input);
	return (head);
}
