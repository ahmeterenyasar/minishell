#include "minishell.h"

static int	check_var_boundary_conflict(char *current_part, char *next_part)
{
	int	current_len;

	current_len = ft_strlen(current_part);
	if (current_len > 0 && current_part[current_len - 1] == '$' && next_part
		&& ft_isalnum(next_part[0]))
		return (1);
	return (0);
}

static int	check_var_at_end(char *current_part, char *next_part)
{
	int	current_len;
	int	k;

	current_len = ft_strlen(current_part);
	if (current_len > 1 && current_part[current_len - 1] != '$')
	{
		k = current_len - 1;
		while (k >= 1 && (current_part[k - 1] != '$'
				|| !ft_isalnum(current_part[k])))
			k--;
		if (k >= 1 && next_part && ft_isalnum(next_part[0]))
			return (1);
	}
	return (0);
}

int	check_boundary_markers_needed(char **word_parts, int part_count)
{
	int		j;
	char	*current_part;
	char	*next_part;

	j = 0;
	while (j < part_count - 1)
	{
		current_part = word_parts[j];
		next_part = word_parts[j + 1];
		if (check_var_boundary_conflict(current_part, next_part))
			return (1);
		if (check_var_at_end(current_part, next_part))
			return (1);
		j++;
	}
	return (0);
}
