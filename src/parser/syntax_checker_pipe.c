#include "minishell.h"
#include "syntax_checker.h"

/**
 * Check pipe operators have valid syntax
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
