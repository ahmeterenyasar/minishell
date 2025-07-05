#include "minishell.h"

char	*convert_newlines(const char *input)
{
	char	*result;
	int		len;
	int		i;
	int		j;

	if (!input)
		return (NULL);
	len = ft_strlen(input);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (input[i])
	{
		if (input[i] == '\\' && input[i + 1] == 'n')
		{
			result[j++] = '\n';
			i += 2;
		}
		else
			result[j++] = input[i++];
	}
	result[j] = '\0';
	return (result);
}

char	**split_commands_by_newlines(const char *input)
{
	char	*converted;
	char	**commands;

	if (!input || !*input)
	{
		commands = malloc(sizeof(char *));
		if (commands)
			commands[0] = NULL;
		return (commands);
	}
	converted = convert_newlines(input);
	if (!converted)
		return (NULL);
	commands = ft_split(converted, '\n');
	free(converted);
	return (commands);
}
