#include "minishell.h"

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
