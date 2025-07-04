#include "minishell.h"
#include "syntax_checker.h"

/**
 * Check redirection token has valid following token
 */
static int	validate_redirection_token(t_token *current)
{
	if (!current->next || current->next->type != TOKEN_WORD)
	{
		if (current->next && is_redirection(current->next->type))
			print_redirection_error(current->next->type);
		else
			print_error("syntax error near unexpected token `newline'\n");
		return (SYNTAX_ERROR);
	}
	return (SUCCESS);
}

/**
 * Check all redirection operators have valid syntax
 */
int	check_redirection_syntax(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		if (is_redirection(current->type))
		{
			if (validate_redirection_token(current) != SUCCESS)
				return (SYNTAX_ERROR);
		}
		current = current->next;
	}
	return (SUCCESS);
}

/**
 * Check for consecutive redirection operators
 */
int	check_consecutive_redirections(t_token *tokens)
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
