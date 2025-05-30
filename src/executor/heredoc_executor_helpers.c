#include "minishell.h"

int	process_cmd_heredocs(t_command *cmd, t_shell_data *shell)
{
	t_redirect	*curr_redir;

	curr_redir = cmd->redirects;
	while (curr_redir)
	{
		if (curr_redir->type == TOKEN_HEREDOC)
		{
			if (process_heredoc(curr_redir, shell))
				return (1);
		}
		curr_redir = curr_redir->next;
	}
	return (0);
}

int	process_all_heredocs(t_command *cmd, t_shell_data *shell)
{
	t_command	*curr_cmd;

	curr_cmd = cmd;
	while (curr_cmd)
	{
		if (process_cmd_heredocs(curr_cmd, shell))
			return (1);
		curr_cmd = curr_cmd->next;
	}
	return (0);
}