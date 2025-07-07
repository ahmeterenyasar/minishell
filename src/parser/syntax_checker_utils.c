/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:53:57 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:53:58 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "syntax_checker.h"

int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

t_token	*skip_empty_tokens(t_token *token)
{
	while (token && token->type == TOKEN_WORD && token->value
		&& is_all_whitespace(token->value))
		token = token->next;
	return (token);
}

void	print_redirection_error(t_token_type type)
{
	if (type == TOKEN_PIPE)
		print_error("syntax error near unexpected token `|'\n");
	else if (type == TOKEN_REDIR_IN)
		print_error("syntax error near unexpected token `<'\n");
	else if (type == TOKEN_REDIR_OUT)
		print_error("syntax error near unexpected token `>'\n");
	else if (type == TOKEN_APPEND)
		print_error("syntax error near unexpected token `>>'\n");
	else if (type == TOKEN_HEREDOC)
		print_error("syntax error near unexpected token `<<'\n");
}
