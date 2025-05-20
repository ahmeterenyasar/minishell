#include "minishell.h"

/**
 * Check if character is a whitespace
 */
int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

char *remove_white_spaces(char *str)
{
	char	*new_str;
	int		i;
	int		j;

	new_str = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!new_str)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if (!is_whitespace(str[i]))
		{
			new_str[j] = str[i];
			j++;
		}
		i++;
	}
	new_str[j] = '\0';
	return (new_str);
}
