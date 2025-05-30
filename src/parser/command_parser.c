#include "minishell.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	initialize_command_args(t_command *cmd, int arg_count)
{
	cmd->args = allocate_args(arg_count);
	if (!cmd->args)
		return (0);
	return (1);
}

t_command	*parse_command(t_token **token_ptr)
{
	t_token		*token;
	t_command	*cmd;
	int			arg_count;

	token = *token_ptr;
	cmd = create_command();
	if (!cmd)
		return (NULL);
	arg_count = count_args(token);
	if (!initialize_command_args(cmd, arg_count))
	{
		free(cmd);
		return (NULL);
	}
	fill_args(cmd, &token, arg_count);
	*token_ptr = token;
	return (cmd);
}

void	add_command(t_command **head, t_command **current, t_command *cmd)
{
	if (!*head)
	{
		*head = cmd;
		*current = *head;
	}
	else
	{
		(*current)->next = cmd;
		*current = (*current)->next;
	}
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_command	*cmd;
	t_token		*token;

	if (!tokens)
		return (NULL);
	head = NULL;
	current = NULL;
	token = tokens;
	while (token)
	{
		cmd = parse_command(&token);
		if (!cmd)
			break;
		add_command(&head, &current, cmd);
		if (token)
		{
			if (token->type == TOKEN_PIPE)
				token = token->next;
		}
	}
	return (head);
}
