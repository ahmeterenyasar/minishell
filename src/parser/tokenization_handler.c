#include "minishell.h"

/**
 * Handles pipe token
 */
int	handle_pipe(const char *input __attribute__((unused)), int i,
		t_token **head)
{
	t_token	*new_token;

	new_token = create_token(TOKEN_PIPE, strdup("|"), 0);
	if (new_token)
		add_token(head, new_token);
	return (i + 1);
}

/**
 * Handles input redirection
 */
int	handle_redir_in(const char *input, int i, t_token **head)
{
	t_token	*new_token;

	if (input[i + 1] == '<')
	{
		new_token = create_token(TOKEN_HEREDOC, strdup("<<"), 0);
		if (new_token)
			add_token(head, new_token);
		return (i + 2);
	}
	else
	{
		new_token = create_token(TOKEN_REDIR_IN, strdup("<"), 0);
		if (new_token)
			add_token(head, new_token);
		return (i + 1);
	}
}

/**
 * Handles output redirection
 */
int	handle_redir_out(const char *input, int i, t_token **head)
{
	t_token	*new_token;

	if (input[i + 1] == '>')
	{
		new_token = create_token(TOKEN_APPEND, strdup(">>"), 0);
		if (new_token)
			add_token(head, new_token);
		return (i + 2);
	}
	else
	{
		new_token = create_token(TOKEN_REDIR_OUT, strdup(">"), 0);
		if (new_token)
			add_token(head, new_token);
		return (i + 1);
	}
}

/**
 * Handle quoted text with better escape sequence handling
 */
int	handle_quotes(const char *input, int i, t_token **head)
{
	char quote_char;
	int start;
	int end;
	char *quoted_text;

	quote_char = input[i++];
	start = i;
	end = find_closing_quote(input, i - 1, quote_char);
	if (end == -1)
	{
		printf("Error: Unclosed quotes\n");
		return (-1);
	}
	quoted_text = malloc(end - start + 1);
	if (!quoted_text)
		return (end + 1);
	copy_quoted_text(input, start, end, quoted_text, quote_char);
	return (create_quoted_token(quote_char, quoted_text, head, end));
}
