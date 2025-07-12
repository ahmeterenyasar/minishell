/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_validator.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: raktas <raktas@student.42istanbul.com.t    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:54:00 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/12 10:02:54 by raktas           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
