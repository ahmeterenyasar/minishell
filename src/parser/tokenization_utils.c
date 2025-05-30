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
		if (input[i] == '\\' && input[i + 1] == 'n')
		{
			result[j++] = '\n';
			i += 2;
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

int	process_token(const char *input, int i, t_token **head)
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
