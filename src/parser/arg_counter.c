#include "minishell.h"

int	is_valid_word_token(t_token *current)
{
	if (current->type != TOKEN_WORD)
		return (0);
	if (!current->value)
		return (0);
	
	// For empty strings, we need to check if they were originally quoted
	// Since we don't have a specific flag for this, we'll treat all empty 
	// TOKEN_WORD tokens as valid (they can only come from quotes)
	if (!*current->value)  // Empty string
	{
		// Empty TOKEN_WORD tokens should be valid (they come from quotes like "" or '')
		return (1);
	}
	
	// Skip tokens that are only whitespace (but not empty)
	if (is_all_whitespace(current->value))
		return (0);
	return (1);
}

t_token	*skip_redirection_tokens(t_token *current)
{
	current = current->next;
	if (current)
	{
		if (current->type == TOKEN_WORD)
			current = current->next;
	}
	return (current);
}

int	count_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_PIPE)
			break;
		if (is_valid_word_token(current))
			count++;
		else if (redir_is_redirection(current))
		{
			current = skip_redirection_tokens(current);
			continue;
		}
		if (current)
			current = current->next;
	}
	return (count);
}

char	**allocate_args(int count)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i <= count)
	{
		args[i] = NULL;
		i++;
	}
	return (args);
}
