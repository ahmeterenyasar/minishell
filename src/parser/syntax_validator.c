/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:54:00 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:54:01 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "syntax_checker.h"

int	check_start_syntax(t_token *tokens)
{
	if (!tokens)
		return (SUCCESS);
	if (tokens->type == TOKEN_PIPE)
	{
		print_error("syntax error near unexpected token `|'\n");
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

int	check_syntax(t_token *tokens)
{
	if (check_start_syntax(tokens) != SUCCESS)
		return (SYNTAX_ERROR);
	if (check_redirection_syntax(tokens) != SUCCESS)
		return (SYNTAX_ERROR);
	if (check_pipe_syntax(tokens) != SUCCESS)
		return (SYNTAX_ERROR);
	if (check_consecutive_redirections(tokens) != SUCCESS)
		return (SYNTAX_ERROR);
	return (SUCCESS);
}
