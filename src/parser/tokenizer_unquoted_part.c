#include "minishell.h"

static void	free_word_parts(char **word_parts, int count)
{
	int	k;

	k = 0;
	while (k < count)
	{
		free(word_parts[k]);
		k++;
	}
}

int	process_unquoted_part(const char *input, int *i, char **word_parts,
		int *part_count, int *expandable)
{
	int	word_start;
	int	word_len;

	word_start = *i;
	while (input[*i] && !is_token_delimiter(input[*i])
		&& !is_operator_char(input[*i]) && !is_quote_char(input[*i]))
	{
		if (input[*i] == '\\' && input[*i + 1])
			*i += 2;
		else
			(*i)++;
	}
	word_len = *i - word_start;
	word_parts[*part_count] = malloc(word_len + 1);
	if (!word_parts[*part_count])
	{
		free_word_parts(word_parts, *part_count);
		return (*i);
	}
	copy_word_chars(input, word_start, word_parts[*part_count], word_len);
	*expandable = 1;
	(*part_count)++;
	return (0);
}
