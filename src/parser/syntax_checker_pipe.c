/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_pipe.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:53:52 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/12 10:02:46 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
