#include "minishell.h"

/**
 * Extracts a word token length considering special chars
 */
int	extract_word_len(const char *input, int i)
{
	int	len;

	len = 0;
	while (input[i + len] && !is_token_delimiter(input[i + len])
		&& !is_quote_char(input[i + len]))
	{
		if (input[i + len] == '\\' && input[i + len + 1])
			len += 2;
		else
			len++;
	}
	return (len);
}

/**
 * Copies characters from input to word, handling escape sequences
 */
static void	copy_word_chars(const char *input, int i, char *word, int len)
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

/**
 * Creates and adds a word token with proper escaping
 */
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

/**
 * Find closing quote character, handling escaped quotes
 * Returns -1 if the closing quote is not found
 */
int	find_closing_quote(const char *input, int i, char quote_char)
{
	i++;
	while (input[i])
	{
		if (quote_char == '"' && input[i] == '\\' && input[i + 1] == '"')
		{
			i += 2;
			continue ;
		}
		if (input[i] == quote_char)
			return (i);
		i++;
	}
	return (-1);
}

/**
 * Copies quoted text, handling escape sequences
 */
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
			if (input[j + 1] == '"' || input[j + 1] == '\\' || input[j
				+ 1] == '$')
			{
				j++;
				quoted_text[k++] = input[j++];
				continue ;
			}
		}
		quoted_text[k++] = input[j++];
	}
	quoted_text[k] = '\0';
}

/**
 * Creates a token from quoted text
 */
int	create_quoted_token(char quote_char, char *quoted_text,
		t_token **head, int end)
{
	t_token	*new_token;
	int		expandable;

	expandable = (quote_char == '"') ? 1 : 0;
	new_token = create_token(TOKEN_WORD, quoted_text, expandable);
	if (new_token)
		add_token(head, new_token);
	else
		free(quoted_text);
	return (end + 1);
}
