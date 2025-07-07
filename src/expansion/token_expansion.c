/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_expansion.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:53:13 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:53:14 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_single_token(t_token *token, t_shell_data *shell)
{
	char	*expanded;

	if (token->type == TOKEN_WORD && token->expandable)
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

	current = tokens;
	while (current)
	{
		expand_single_token(current, shell);
		current = current->next;
	}
}
