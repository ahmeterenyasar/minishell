#include "minishell.h"

void	copy_quoted_text(const char *input, int start, int end,
		char *quoted_text, char quote_char)
{
	int		j;
	int		k;

	j = start;
	k = 0;
	while (j < end)
	{
		if (quote_char == '"' && input[j] == '\\' && j + 1 < end)
		{
			if (should_escape_char(input[j + 1]))
			{
				j++;
				quoted_text[k++] = input[j++];
				continue;
			}
		}
		quoted_text[k++] = input[j++];
	}
	quoted_text[k] = '\0';
}

int	should_escape_char(char c)
{
	if (c == '"')
		return (1);
	if (c == '\\')
		return (1);
	if (c == '$')
		return (1);
	return (0);
}

int	create_quoted_token(char quote_char, char *quoted_text,
		t_token **head, int end)
{
	t_token	*new_token;
	int		expandable;

	if (quote_char == '"')
		expandable = 1;
	else
		expandable = 0;
	new_token = create_token(TOKEN_WORD, quoted_text, expandable);
	if (new_token)
		add_token(head, new_token);
	else
		free(quoted_text);
	return (end + 1);
}
