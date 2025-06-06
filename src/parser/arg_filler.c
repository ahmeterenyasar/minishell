#include "minishell.h"

int	should_add_word_arg(t_token *token)
{
	if (token->type != TOKEN_WORD)
		return (0);
	if (!token->value)
		return (0);
	
	// For empty strings, treat them as valid arguments
	// Empty TOKEN_WORD tokens come from quoted empty strings like "" or ''
	if (!*token->value)  // Empty string
	{
		return (1);  // Keep empty quoted strings as valid arguments
	}
	
	// Skip tokens that are only whitespace (but not empty)
	if (is_all_whitespace(token->value))
		return (0);
	return (1);
}

t_token	*process_word_token(t_token *token, t_command *cmd, int *arg_index)
{
	if (should_add_word_arg(token))
	{
		cmd->args[*arg_index] = ft_strdup(token->value);
		if (!cmd->args[*arg_index])  // Handle strdup failure
		{
			// Could set an error flag here
			return (token->next);
		}
		(*arg_index)++;
	}
	// Skip empty tokens by just advancing to next without adding
	return (token->next);
}

t_token *process_parser_token(t_token *token, t_command *cmd, int *arg_index)
{
	if (token->type == TOKEN_WORD)
		return (process_word_token(token, cmd, arg_index));
	else if (redir_is_redirection(token))
		return (handle_redirection_arg(token, cmd));
	else
		return (token->next);
}

void	fill_args(t_command *cmd, t_token **tokens, int arg_count)
{
	t_token	*token;
	int		arg_index;

	token = *tokens;
	arg_index = 0;
	while (token)
	{
		if (token->type == TOKEN_PIPE)
			break;

		token = process_parser_token(token, cmd, &arg_index);

		if (!token)
			break;

		if (arg_index >= arg_count)
		{
			while (token && token->type != TOKEN_PIPE)
			{
				if (redir_is_redirection(token))
					token = handle_redirection_arg(token, cmd);
				else
					token = token->next;
			}
			break;
		}
	}
	*tokens = token;
}
