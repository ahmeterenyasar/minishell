#include "minishell.h"

void	free_str_array(char **arr)
{
	int i = 0;
	if (!arr)
		return;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}


int	is_all_whitespace(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!is_white_space(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

int	is_quote_char(char c)
{
	return (c == '\'' || c == '"');
}

int	is_token_delimiter(char c)
{
	return (isspace(c) || is_operator_char(c) || c == '\0');
}
