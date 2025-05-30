#include "minishell.h"

t_redirect	*parse_redirection(t_token **token_ptr)
{
	t_token		*token;
	t_redirect	*redir;

	token = *token_ptr;
	if (!is_valid_redir_token(token))
		return (NULL);
	redir = create_redirection(token);
	if (!redir)
		return (NULL);
	*token_ptr = token->next;
	token = *token_ptr;
	if (!set_redirection_file(redir, token))
	{
		free(redir);
		return (NULL);
	}
	*token_ptr = token->next;
	return (redir);
}

t_token	*handle_redirection_arg(t_token *token, t_command *cmd)
{
	t_redirect	*redir;
	t_token		*before_redir;

	before_redir = token;
	redir = parse_redirection(&token);
	if (redir)
		add_redirection(cmd, redir);
	else if (token == before_redir)
		token = token->next;
	return (token);
}
