#include "minishell.h"

/**
 * Check if token is a redirection
 */
int	is_redirection(t_token_type type)
{
	return (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC);
}

/**
 * Check if syntax is valid at the start of input
 */
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

/**
 * Check if redirection has a valid file name
 */
int	check_redirection_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (is_redirection(current->type))
		{
			if (!current->next || current->next->type != TOKEN_WORD)
			{
				if (current->next && is_redirection(current->next->type))
				{
					if (current->next->type == TOKEN_PIPE)
						print_error("syntax error near unexpected token `|'\n");
					else if (current->next->type == TOKEN_REDIR_IN)
						print_error("syntax error near unexpected token `<'\n");
					else if (current->next->type == TOKEN_REDIR_OUT)
						print_error("syntax error near unexpected token `>'\n");
					else if (current->next->type == TOKEN_APPEND)
						print_error("syntax error near unexpected token `>>'\n");
					else if (current->next->type == TOKEN_HEREDOC)
						print_error("syntax error near unexpected token `<<'\n");
				}
				else
					print_error("syntax error near unexpected token `newline'\n");
				return (SYNTAX_ERROR);
			}
		}
		current = current->next;
	}
	return (SUCCESS);
}

/**
 * Skip empty or whitespace-only tokens
 */
t_token	*skip_empty_tokens(t_token *token)
{
	while (token && token->type == TOKEN_WORD && token->value
		&& is_all_whitespace(token->value))
		token = token->next;
	return (token);
}

/**
 * Check for misplaced pipes
 */
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

/**
 * Check for consecutive redirections
 */
static int	check_consecutive_redirections(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current && current->next)
	{
		if (is_redirection(current->type)
			&& is_redirection(current->next->type))
		{
			print_error("syntax error near unexpected token `");
			print_error(current->next->value);
			print_error("'\n");
			return (SYNTAX_ERROR);
		}
		current = current->next;
	}
	return (SUCCESS);
}

/**
 * Check entire token list for syntax errors
 */
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
