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
		free_tokens(tokens); // ✅ Tokens freed on syntax error
		set_exit_status(shell, 2);
		return (NULL);
	}
	
	expand_tokens(tokens, shell);
	cmd = parse_tokens(tokens);
	
	// ✅ CRITICAL: Always free tokens after parsing
	free_tokens(tokens);
	tokens = NULL;
	
	if (cmd)
	{
		// Expand redirection files after command parsing
		expand_command_redirects(cmd, shell);
	}
	
	return (cmd);
}
