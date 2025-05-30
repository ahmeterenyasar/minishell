#include "minishell.h"

int	extract_word_len(const char *input, int i)
{
	int	len;

	len = 0;
	while (input[i + len])
	{
		if (is_token_delimiter(input[i + len]))
			break;
		if (is_quote_char(input[i + len]))
			break;
		if (input[i + len] == '\\' && input[i + len + 1])
			len += 2;
		else
			len++;
	}
	return (len);
}

void	copy_word_chars(const char *input, int i, char *word, int len)
{
	int		j;
	int		k;

	j = i;
	k = 0;
	while (j < i + len)
	{
		if (input[j] == '\\' && j + 1 < i + len)
			word[k++] = input[++j];
		else
			word[k++] = input[j];
		j++;
	}
	word[k] = '\0';
}

int	add_word_token(const char *input, int i, t_token **head)
{
	int		len;
	char	*word;
	t_token	*new_token;

	len = extract_word_len(input, i);
	word = malloc(len + 1);
	if (!word)
		return (i + len);
	copy_word_chars(input, i, word, len);
	new_token = create_token(TOKEN_WORD, word, 1);
	if (new_token)
		add_token(head, new_token);
	else
		free(word);
	return (i + len);
}

int	find_closing_quote(const char *input, int i, char quote_char)
{
	i++;
	while (input[i])
	{
		if (quote_char == '"' && input[i] == '\\' && input[i + 1] == '"')
		{
			i += 2;
			continue;
		}
		if (input[i] == quote_char)
			return (i);
		i++;
	}
	return (-1);
}
