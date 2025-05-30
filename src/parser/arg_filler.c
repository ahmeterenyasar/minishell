#include "minishell.h"

int	should_add_word_arg(t_token *token)
{
	if (token->type != TOKEN_WORD)
		return (0);
	if (!token->value)
		return (0);
	if (!*token->value)
		return (0);
	if (is_all_whitespace(token->value))
		return (0);
	return (1);
}

t_token	*process_word_token(t_token *token, t_command *cmd, int *arg_index)
{
	if (should_add_word_arg(token))
	{
		cmd->args[*arg_index] = ft_strdup(token->value);
		(*arg_index)++;
	}
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
		if (arg_index >= arg_count)
			break;
		token = process_parser_token(token, cmd, &arg_index);
	}
	*tokens = token;
}
