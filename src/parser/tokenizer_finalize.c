#include "minishell.h"

int	finalize_concatenated_word(char **word_parts, int part_count,
		int start_pos, int expandable, t_token **head,
		const char *input, int i)
{
	int		need_boundary_markers;
	char	*concatenated;
	t_token	*new_token;

	if (part_count == 1 && is_quote_char(input[start_pos]))
		return (handle_single_quoted_token(input, start_pos,
				word_parts, head) ? i : i);
	need_boundary_markers = check_boundary_markers_needed(word_parts,
			part_count);
	concatenated = concatenate_word_parts(word_parts, part_count,
			need_boundary_markers);
	if (!concatenated)
		return (i);
	new_token = create_token(TOKEN_WORD, concatenated, expandable);
	if (new_token)
		add_token(head, new_token);
	else
		free(concatenated);
	return (i);
}
