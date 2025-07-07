/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_creation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:55:32 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:55:33 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(t_token_type type, char *value, int expandable)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		if (value)
			free(value);
		return (NULL);
	}
	token->type = type;
	token->value = value;
	token->expandable = expandable;
	token->quoted = 0;
	token->next = NULL;
	return (token);
}

t_token	*create_quoted_token_with_flag(t_token_type type, char *value,
		int expandable, int quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		if (value)
			free(value);
		return (NULL);
	}
	token->type = type;
	token->value = value;
	token->expandable = expandable;
	token->quoted = quoted;
	token->next = NULL;
	return (token);
}
