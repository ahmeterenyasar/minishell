#include "minishell.h"

char	*remove_quotes_from_delimiter(char *delimiter)
{
	int		len;
	char	*result;

	int i, j;
	if (!delimiter)
		return (NULL);
	len = ft_strlen(delimiter);
	if (len >= 2 && ((delimiter[0] == '"' && delimiter[len - 1] == '"')
			|| (delimiter[0] == '\'' && delimiter[len - 1] == '\'')))
	{
		result = malloc(len - 1);
		if (!result)
			return (NULL);
		i = 1;
		j = 0;
		while (i < len - 1)
			result[j++] = delimiter[i++];
		result[j] = '\0';
		return (result);
	}
	return (ft_strdup(delimiter));
}

int	is_delimiter_quoted(char *delimiter)
{
	int	len;

	if (!delimiter)
		return (0);
	len = ft_strlen(delimiter);
	if (len >= 2 && ((delimiter[0] == '"' && delimiter[len - 1] == '"')
			|| (delimiter[0] == '\'' && delimiter[len - 1] == '\'')))
		return (1);
	return (0);
}

int	validate_heredoc_redirect(t_redirect *redirect)
{
	if (!redirect || redirect->type != TOKEN_HEREDOC)
	{
		write(2, "minishell: invalid heredoc redirect\n", 35);
		return (1);
	}
	return (0);
}
