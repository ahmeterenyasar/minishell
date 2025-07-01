#include "minishell.h"

int	is_valid_redir_token(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == TOKEN_REDIR_IN)
		return (1);
	if (token->type == TOKEN_REDIR_OUT)
		return (1);
	if (token->type == TOKEN_APPEND)
		return (1);
	if (token->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

int	redir_is_redirection(t_token *token)
{
	if (token->type == TOKEN_REDIR_IN)
		return (1);
	if (token->type == TOKEN_REDIR_OUT)
		return (1);
	if (token->type == TOKEN_APPEND)
		return (1);
	if (token->type == TOKEN_HEREDOC)
		return (1);
	return (0);
}

void	add_redirection(t_command *cmd, t_redirect *redir)
{
	t_redirect	*current;

	if (!cmd->redirects)
	{
		cmd->redirects = redir;
		return ;
	}
	current = cmd->redirects;
	while (current->next)
		current = current->next;
	current->next = redir;
}

t_redirect	*create_redirection(t_token *token)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = token->type;
	redir->expand_heredoc = 1;
	redir->was_quoted = 0;
	redir->next = NULL;
	return (redir);
}

int	set_redirection_file(t_redirect *redir, t_token *token)
{
	if (!token)
		return (0);
	if (token->type != TOKEN_WORD)
		return (0);
	redir->file = ft_strdup(token->value);
	if (!redir->file)
		return (0);
	// Track if the filename was originally quoted
	redir->was_quoted = token->quoted;
	if (redir->type == TOKEN_HEREDOC)
	{
		// For heredoc expansion rules (following bash behavior):
		// - Unquoted delimiter: Variables ARE expanded in heredoc content
		// - Single quoted delimiter: Variables are NOT expanded in heredoc content
		// - Double quoted delimiter: Variables are NOT expanded in heredoc content
		if (token->quoted)
			redir->expand_heredoc = 0; // Any quoted delimiter - no expansion
		else
			redir->expand_heredoc = 1; // Unquoted delimiter - expansion
	}
	return (1);
}
