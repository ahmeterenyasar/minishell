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

int	process_quoted_part(const char *input, int *i, char **word_parts,
		int *part_count, int *expandable)
{
	char	quote_char;
	int		quote_start;
	int		quote_end;
	int		content_len;

	quote_char = input[*i];
	quote_start = *i + 1;
	quote_end = find_closing_quote(input, *i, quote_char);
	if (quote_end == -1)
	{
		free_word_parts(word_parts, *part_count);
		return (-1);
	}
	content_len = quote_end - quote_start;
	word_parts[*part_count] = malloc(content_len + 1);
	if (!word_parts[*part_count])
	{
		free_word_parts(word_parts, *part_count);
		return (quote_end + 1);
	}
	copy_quoted_text(input, quote_start, quote_end,
		word_parts[*part_count], quote_char);
	if (quote_char == '"')
		*expandable = 1;
	(*part_count)++;
	*i = quote_end + 1;
	return (0);
}
