/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_single_token.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:56:00 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/11 21:25:03 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_single_quoted_token(const char *input, int start_pos,
		char **word_parts, t_token **head)
{
	int		expandable_flag;
	t_token	*new_token;

	if (input[start_pos] == '"')
		expandable_flag = 1;
	else
		expandable_flag = 0;
	new_token = create_token(TOKEN_WORD, word_parts[0],
			expandable_flag, 1);
	if (new_token)
		add_token(head, new_token);
	else
		free(word_parts[0]);
	return (1);
}

int	calculate_total_length(char **word_parts, int part_count)
{
	int	total_len;
	int	j;

	total_len = 0;
	j = 0;
	while (j < part_count)
		total_len += ft_strlen(word_parts[j++]);
	return (total_len);
}
