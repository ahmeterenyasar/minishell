#include "minishell.h"

/**
 * Print token type name
 */
const char	*get_token_type_name(t_token_type type)
{
	const char	*names[] = {"WORD", "PIPE", "REDIR_IN", "REDIR_OUT", "APPEND",
			"HEREDOC"};

	if (type >= 0 && type <= TOKEN_HEREDOC)
		return (names[type]);
	return ("UNKNOWN");
}

/**
 * Print the list of tokens for debugging
 */
void	print_tokens(t_token *tokens)
{
	t_token	*current;

	printf("Tokens:\n");
	current = tokens;
	while (current)
	{
		printf("  Type: %-10s Value: '%s' Expandable: %d\n",
			get_token_type_name(current->type), current->value,
			current->expandable);
		current = current->next;
	}
	printf("\n");
}

/**
 * Print command arguments
 */
void	print_command_args(char **args)
{
	int	i;

	printf("  Arguments: ");
	i = 0;
	while (args[i])
	{
		printf("%s", args[i]);
		i++;
	}
	printf("\n");
}

/**
 * Print command redirections
 */
void	print_command_redirects(t_redirect *redirects)
{
	t_redirect	*current;

	if (!redirects)
		return ;
	printf("  Redirections:\n");
	current = redirects;
	while (current)
	{
		printf("    Type: %-10s File: '%s'", get_token_type_name(current->type),
			current->file);
		if (current->type == TOKEN_HEREDOC)
			printf(" Expand: %d", current->expand_heredoc);
		printf("\n");
		current = current->next;
	}
}

/**
 * Print the command structure for debugging
 */
void	print_command(t_command *cmd)
{
	t_command	*current;
	int			cmd_count;

	printf("Command Structure:\n");
	cmd_count = 0;
	current = cmd;
	while (current)
	{
		printf("Command %d:\n", ++cmd_count);
		print_command_args(current->args);
		print_command_redirects(current->redirects);
		current = current->next;
		if (current)
			printf("  | (pipe)\n");
	}
	printf("\n");
}
