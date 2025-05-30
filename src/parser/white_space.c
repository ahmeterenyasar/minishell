#include "minishell.h"

int	is_white_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

int	skip_white_space(const char *input, int i)
{
	while (input[i] && is_white_space(input[i]))
		i++;
	return (i);
}

