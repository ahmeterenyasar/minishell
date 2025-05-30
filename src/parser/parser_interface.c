#include "minishell.h"

t_command	*parse_input(const char *line, t_shell_data *shell)
{
	t_token		*tokens;
	t_command	*cmd;

	if (!line || !*line)
		return (NULL);
	tokens = tokenize(line);
	if (!tokens)
		return (NULL);
	if (check_syntax(tokens) != SUCCESS)
	{
		free_tokens(tokens);
		set_exit_status(shell, 2); // Syntax error exit code
		return (NULL);
	}
	expand_tokens(tokens, shell);
	cmd = parse_tokens(tokens);
	free_tokens(tokens);
	
	// Expand redirection files after command parsing
	// NOTE: Heredoc işlemi burada YAPILMIYOR - sadece executor'da yapılacak
	if (cmd)
		expand_command_redirects(cmd, shell);
	
	return (cmd);
}