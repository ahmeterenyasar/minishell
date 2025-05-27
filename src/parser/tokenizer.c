#include "minishell.h"

// düzenlencek
static char	*handle_newlines(const char *input)
{
	char	*result;
	int		len;

	int i;
	int j;
	if (!input)
		return (NULL);
	len = strlen(input);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '\\' && input[i + 1] == 'n')
		{
			result[j++] = '\n';
			i += 2;
		}
		else
		{
			result[j++] = input[i++];
		}
	}
	result[j] = '\0';
	return (result);
}

static int	process_token(const char *input, int i, t_token **head)
{
	int	result;

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

t_token	*tokenize(const char *input)
{
	t_token *head;
	int i;
	int result;
	char *processed_input;

	processed_input = handle_newlines(input);
	if (!processed_input)
		return (NULL);

	head = NULL;
	i = 0;
	while (processed_input[i])
	{
		i = skip_white_space(processed_input, i);
		if (!processed_input[i])
			break ;
		result = process_token(processed_input, i, &head);
		if (result == -1)
		{
			free(processed_input);
			return (NULL);
		}
		i = result;
	}

	free(processed_input);
	return (head);
}
