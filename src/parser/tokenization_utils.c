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
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

// Modified to NOT convert \n to actual newlines in tokenization phase
char	*handle_newlines(const char *input)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		// Keep \n as literal for now - will be handled at execution level
		result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

int	should_use_concatenation(const char *input, int i)
{
	int pos = i;
	int transitions = 0;
	int in_quotes = 0;
	int was_in_quotes = 0;
	
	// Scan through the current word to count quote/unquote transitions
	while (input[pos] && !is_token_delimiter(input[pos]) && !is_operator_char(input[pos]))
	{
		if (is_quote_char(input[pos]))
		{
			if (!in_quotes)
			{
				// Entering quotes
				if (pos > i && !was_in_quotes) // There was unquoted text before
					transitions++;
				in_quotes = 1;
				was_in_quotes = 1;
				// Find the closing quote
				char quote_char = input[pos];
				pos = find_closing_quote(input, pos, quote_char);
				if (pos == -1)
					return (0); // Unclosed quote
				pos++; // Move past closing quote
				in_quotes = 0;
			}
		}
		else
		{
			// Unquoted character
			if (was_in_quotes && !in_quotes)
			{
				// Coming out of quotes to unquoted text
				transitions++;
			}
			// Skip through unquoted text
			while (input[pos] && !is_token_delimiter(input[pos]) && 
				   !is_operator_char(input[pos]) && !is_quote_char(input[pos]))
			{
				if (input[pos] == '\\' && input[pos + 1])
					pos += 2;
				else
					pos++;
			}
		}
	}
	
	// If we had any transitions, we need concatenation
	return (transitions > 0);
}



// Keep your existing process_token function - don't change it!
// The version you have already handles concatenated words correctly

int	process_token(const char *input, int i, t_token **head)
{
	// First check if we need concatenation by looking ahead
	if (should_use_concatenation(input, i))
		return (process_concatenated_word(input, i, head));
	
	// Otherwise handle single tokens as before
	if (input[i] == '\'' || input[i] == '"')
	{
		int result = handle_quotes(input, i, head);
		if (result == -1)
		{
			free_tokens(*head);
			return (-1);
		}
		return (result);
	}
	else if (input[i] == '|')
		return (handle_pipe(input, i, head));
	else if (input[i] == '<')
		return (handle_redir_in(input, i, head));
	else if (input[i] == '>')
		return (handle_redir_out(input, i, head));
	else
		return (add_word_token(input, i, head));
}