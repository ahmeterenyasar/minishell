#include "minishell.h"

static int	handle_input_redirections(t_redirect *redirect)
{
	if (redirect->type == TOKEN_REDIR_IN || redirect->type == TOKEN_HEREDOC)
		return (setup_input_redirection(redirect));
	return (0);
}

static int	handle_output_redirections(t_redirect *redirect)
{
	if (redirect->type == TOKEN_REDIR_OUT || redirect->type == TOKEN_APPEND)
		return (setup_output_redirection(redirect));
	return (0);
}

static int	process_single_redirection(t_redirect *redirect)
{
	if (validate_redirection_file(redirect) == -1)
		return (-1);
	if (handle_input_redirections(redirect) == -1)
		return (-1);
	if (handle_output_redirections(redirect) == -1)
		return (-1);
	return (0);
}

int	setup_redirections(t_redirect *redirects)
{
	t_redirect	*current;

	current = redirects;
	while (current)
	{
		if (process_single_redirection(current) == -1)
			return (-1);
		current = current->next;
	}
	return (0);
}
