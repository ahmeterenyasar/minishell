/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_concatenate.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:53 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:55:54 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_variable_at_end(char *part)
{
	int	len;
	int	k;

	len = ft_strlen(part);
	if (len > 0 && part[len - 1] == '$')
		return (1);
	k = len - 1;
	while (k >= 1)
	{
		if (part[k - 1] == '$' && ft_isalnum(part[k]))
			return (1);
		if (!ft_isalnum(part[k]) && part[k] != '_')
			break ;
		k--;
	}
	return (0);
}

char	*concatenate_word_parts(char **word_parts, int part_count,
		int need_boundary_markers)
{
	char	*concatenated;
	int		total_len;
	int		j;

	total_len = calculate_total_length(word_parts, part_count);
	concatenated = malloc(total_len + part_count * 2 + 1);
	if (!concatenated)
		return (NULL);
	concatenated[0] = '\0';
	j = 0;
	while (j < part_count)
	{
		ft_strcat(concatenated, word_parts[j]);
		if (need_boundary_markers && j < part_count - 1
			&& has_variable_at_end(word_parts[j]) && word_parts[j + 1]
			&& ft_isalnum(word_parts[j + 1][0]))
			ft_strcat(concatenated, "\x01");
		free(word_parts[j]);
		j++;
	}
	return (concatenated);
}
