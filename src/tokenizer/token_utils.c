/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:43 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:55:44 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*handle_newlines(const char *input)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
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
