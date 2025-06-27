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
	token->quoted = 0; // Default to not quoted
	token->next = NULL;
	return (token);
}

t_token	*create_quoted_token_with_flag(t_token_type type, char *value,
		int expandable, int quoted)
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
	token->quoted = quoted;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
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
	int		pos;
	int		quote_segments;
	int		has_unquoted;
	int		in_quotes;
	char	quote_char;

	pos = i;
	quote_segments = 0;
	has_unquoted = 0;
	in_quotes = 0;
	// Scan through the current word to check for multiple segments
	while (input[pos] && !is_token_delimiter(input[pos])
		&& !is_operator_char(input[pos]))
	{
		if (is_quote_char(input[pos]))
		{
			if (!in_quotes)
			{
				// Entering quotes
				quote_segments++;
				in_quotes = 1;
				// Find the closing quote
				quote_char = input[pos];
				pos = find_closing_quote(input, pos, quote_char);
				if (pos == -1)
					return (0); // Unclosed quote
				pos++;          // Move past closing quote
				in_quotes = 0;
			}
		}
		else
		{
			// Unquoted character
			has_unquoted = 1;
			// Skip through unquoted text
			while (input[pos] && !is_token_delimiter(input[pos])
				&& !is_operator_char(input[pos]) && !is_quote_char(input[pos]))
			{
				if (input[pos] == '\\' && input[pos + 1])
					pos += 2;
				else
					pos++;
			}
		}
	}
	// We need concatenation if:
	// 1. Multiple quote segments, OR
	// 2. Mix of quoted and unquoted segments
	return (quote_segments > 1 || (quote_segments > 0 && has_unquoted));
}

int	process_token(const char *input, int i, t_token **head)
{
	int	result;

	// First check if we need concatenation by looking ahead
	if (should_use_concatenation(input, i))
		return (process_concatenated_word(input, i, head));
	// Otherwise handle single tokens as before
	if (input[i] == '\'' || input[i] == '"')
	{
		result = handle_quotes(input, i, head);
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
