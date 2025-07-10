/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:53:13 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/10 16:16:41 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_heredoc_delimiter(t_token *current, t_token *prev)
{
	if (!current || !prev)
		return (0);
	if (current->type == TOKEN_WORD && prev->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

static void	expand_single_token(t_token *token, t_token *prev,
		t_shell_data *shell)
{
	char	*expanded;

	if (token->type == TOKEN_WORD && token->expandable
		&& !is_heredoc_delimiter(token, prev))
	{
		expanded = expand_concatenated_vars(token->value, shell);
		if (expanded)
		{
			free(token->value);
			token->value = expanded;
		}
	}
}

void	expand_tokens(t_token *tokens, t_shell_data *shell)
{
	t_token	*current;
	t_token	*prev;

	current = tokens;
	prev = NULL;
	while (current)
	{
		expand_single_token(current, prev, shell);
		prev = current;
		current = current->next;
	}
}
