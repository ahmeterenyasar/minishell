#include "minishell.h"

static int	handle_quote_error(char **word_parts, int part_count, int quote_end)
{
	free_word_parts(word_parts, part_count);
	if (quote_end == -1)
		return (-1);
	return (quote_end + 1);
}

static void	set_expandable_flag(char quote_char, int *expandable)
{
	if (quote_char == '"')
		*expandable = 1;
}

static int	prepare_quoted_content(char **word_parts, int part_count,
		int content_len, int quote_end)
{
	if (!allocate_word_part(word_parts, part_count, content_len))
		return (handle_quote_error(word_parts, part_count, quote_end));
	return (0);
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
		return (handle_quote_error(word_parts, *part_count, quote_end));
	content_len = quote_end - quote_start;
	if (prepare_quoted_content(word_parts, *part_count, content_len,
			quote_end) != 0)
		return (quote_end + 1);
	copy_quoted_text(input, quote_start, quote_end, word_parts[*part_count],
			quote_char);
	set_expandable_flag(quote_char, expandable);
	finalize_word_part(part_count);
	*i = quote_end + 1;
	return (0);
}
