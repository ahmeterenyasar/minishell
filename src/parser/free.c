#include "minishell.h"

void free_tokens(t_token *tokens)
{
	t_token *tmp;

	while (tokens)
	{
		tmp = tokens;
		tokens = tokens->next;
		free(tmp->value);
		free(tmp);
	}
}

void free_redirect(t_redirect *redirect)
{
	t_redirect *next;
	
	while (redirect)
	{
		next = redirect->next;
		free(redirect->file);
		free(redirect);
		redirect = next;
	}
}

void free_command(t_command *cmd)
{
	t_command *next;
	int i;
	
	while (cmd)
	{
		next = cmd->next;
		
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		
		free_redirect(cmd->redirects);
		free(cmd);
		cmd = next;
	}
}
