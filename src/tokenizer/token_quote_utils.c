/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_quote_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:41 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 12:19:34 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	copy_quoted_text(const char *input, t_quote_pos pos, char *quoted_text)
{
	int	j;
	int	k;

	j = pos.start;
	k = 0;
	while (j < pos.end)
	{
		if (pos.quote_char == '"' && input[j] == '\\' && j + 1 < pos.end)
		{
			if (should_escape_char(input[j + 1]))
			{
				j++;
				quoted_text[k++] = input[j++];
				continue ;
			}
		}
		quoted_text[k++] = input[j++];
	}
	quoted_text[k] = '\0';
}

int	should_escape_char(char c)
{
	if (c == '"')
		return (1);
	if (c == '\\')
		return (1);
	if (c == '$')
		return (1);
	return (0);
}

int	create_quoted_token(char quote_char, char *quoted_text, t_token **head,
		int end)
{
	t_token	*new_token;
	int		expandable;

	if (quote_char == '"')
		expandable = 1;
	else
		expandable = 0;
	new_token = create_quoted_token_with_flag(TOKEN_WORD, quoted_text,
			expandable, 1);
	if (new_token)
		add_token(head, new_token);
	else
		free(quoted_text);
	return (end + 1);
}
