/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_finalize.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:55 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 13:22:43 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	finalize_concatenated_word(char **word_parts, int part_count,
		t_finalize_params params, t_token **head)
{
	int		need_boundary_markers;
	char	*concatenated;
	t_token	*new_token;

	if (part_count == 1 && is_quote_char(params.input[params.start_pos]))
	{
		if (handle_single_quoted_token(params.input, params.start_pos,
				word_parts, head))
			return (params.current_pos);
		return (params.current_pos);
	}
	need_boundary_markers = check_boundary_markers_needed(word_parts,
			part_count);
	concatenated = concatenate_word_parts(word_parts, part_count,
			need_boundary_markers);
	if (!concatenated)
		return (params.current_pos);
	new_token = create_token(TOKEN_WORD, concatenated, params.expandable);
	if (new_token)
		add_token(head, new_token);
	else
		free(concatenated);
	return (params.current_pos);
}
