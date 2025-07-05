#include "minishell.h"

static char	*expand_env_vars_in_redirects(const char *str, t_shell_data *shell)
{
	char	*result;
	char	var_name[256];

	if (!str)
		return (ft_strdup(""));
	result = allocate_result_buffer();
	if (!result)
		return (ft_strdup(""));
	expand_loop(str, result, var_name, shell);
	return (result);
}

static void	expand_single_redirect(t_redirect *redirect, t_shell_data *shell)
{
	char	*expanded;

	if (redirect->file && redirect->type != TOKEN_HEREDOC)
	{
		expanded = expand_env_vars_in_redirects(redirect->file, shell);
		if (expanded)
		{
			free(redirect->file);
			redirect->file = expanded;
		}
	}
}

static void	expand_redirect_files(t_redirect *redirects, t_shell_data *shell)
{
	t_redirect	*current;

	current = redirects;
	while (current)
	{
		expand_single_redirect(current, shell);
		current = current->next;
	}
}

void	expand_command_redirects(t_command *cmd, t_shell_data *shell)
{
	t_command	*current;

	current = cmd;
	while (current)
	{
		if (current->redirects)
			expand_redirect_files(current->redirects, shell);
		current = current->next;
	}
}
