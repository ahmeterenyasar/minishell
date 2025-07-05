#include "minishell.h"

void	free_word_parts(char **word_parts, int count)
{
	int	k;

	k = 0;
	while (k < count)
	{
		free(word_parts[k]);
		k++;
	}
}

int	handle_word_allocation_error(char **word_parts, int part_count)
{
	free_word_parts(word_parts, part_count);
	return (-1);
}

int	allocate_word_part(char **word_parts, int part_count, int length)
{
	word_parts[part_count] = malloc(length + 1);
	if (!word_parts[part_count])
		return (0);
	return (1);
}

void	finalize_word_part(int *part_count)
{
	(*part_count)++;
}
