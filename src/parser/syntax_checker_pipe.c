/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:53:52 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:53:53 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "syntax_checker.h"

int	check_pipe_syntax(t_token *tokens)
{
	t_token	*current;
	t_token	*next;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
		{
			next = skip_empty_tokens(current->next);
			if (!next || next->type == TOKEN_PIPE)
			{
				print_error("syntax error near unexpected token `|'\n");
				return (SYNTAX_ERROR);
			}
		}
		current = current->next;
	}
	return (SUCCESS);
}
