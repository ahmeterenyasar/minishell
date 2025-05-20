#include "minishell.h"

t_command	*parser_interface(const char *input, char **envp)
{
	(void)envp;
	/*
	tokenize et
	expand et
	parse et
	*/
	t_token *token_list;

	if (!input || !*input)
		return (NULL);

	// Tokenize the input
	token_list = tokenize(input);
	if (!token_list)
	{
		printf("minishell: TOKEN error\n");
		return (NULL);
	}
	return (NULL);
}