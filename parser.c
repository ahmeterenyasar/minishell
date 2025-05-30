#include "minishell.h"

int	is_valid_redir_token(t_token *token)
{
	return (token && (token->type == TOKEN_REDIR_IN
			|| token->type == TOKEN_REDIR_OUT || token->type == TOKEN_APPEND
			|| token->type == TOKEN_HEREDOC));
}

t_redirect	*parse_redirection(t_token **token_ptr)
{
	t_token		*token;
	t_redirect	*redir;

	token = *token_ptr;
	if (!is_valid_redir_token(token))
		return (NULL);
	redir = malloc(sizeof(t_redirect));
	if (!redir)
		return (NULL);
	redir->type = token->type;
	redir->expand_heredoc = 1;
	*token_ptr = token->next;
	token = *token_ptr;
	if (!token || token->type != TOKEN_WORD)
	{
		free(redir);
		return (NULL);
	}
	redir->file = strdup(token->value);
	redir->next = NULL;
	if (redir->type == TOKEN_HEREDOC && !token->expandable)
		redir->expand_heredoc = 0;
	*token_ptr = token->next;
	return (redir);
}

void	add_redirection(t_command *cmd, t_redirect *redir)
{
	t_redirect	*current;

	if (!cmd->redirects)
		cmd->redirects = redir;
	else
	{
		current = cmd->redirects;
		while (current->next)
			current = current->next;
		current->next = redir;
	}
}

int	redir_is_redirection(t_token *token)
{
	return (token->type == TOKEN_REDIR_IN || token->type == TOKEN_REDIR_OUT
		|| token->type == TOKEN_APPEND || token->type == TOKEN_HEREDOC);
}

int	count_args(t_token *tokens)
{
	int		count;
	t_token	*current;

	count = 0;
	current = tokens;
	while (current && current->type != TOKEN_PIPE)
	{
		if (current->type == TOKEN_WORD)
		{
			// Don't count empty or whitespace-only arguments
			if (current->value && *current->value
				&& !is_all_whitespace(current->value))
				count++;
		}
		else if (is_redirection(current))
		{
			current = current->next;
			if (current && current->type == TOKEN_WORD)
				current = current->next;
			continue ;
		}
		if (current)
			current = current->next;
	}
	return (count);
}

char	**allocate_args(int count)
{
	char	**args;
	int		i;

	args = malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (i <= count)
	{
		args[i] = NULL;
		i++;
	}
	return (args);
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

void	fill_args(t_command *cmd, t_token **tokens, int arg_count)
{
	t_token	*token;
	int		arg_index;

	token = *tokens;
	arg_index = 0;
	while (token && token->type != TOKEN_PIPE && arg_index < arg_count)
	{
		if (token->type == TOKEN_WORD)
		{
			// Skip empty or whitespace-only arguments
			if (token->value && *token->value
				&& !is_all_whitespace(token->value))
			{
				cmd->args[arg_index++] = ft_strdup(token->value);
			}
			token = token->next;
		}
		else if (is_redirection(token))
			token = handle_redirection_arg(token, cmd);
		else
			token = token->next;
	}
	*tokens = token;
}

t_command	*parse_command(t_token **token_ptr)
{
	t_token		*token;
	t_command	*cmd;
	int			arg_count;

	token = *token_ptr;
	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->redirects = NULL;
	cmd->next = NULL;
	arg_count = count_args(token);
	cmd->args = allocate_args(arg_count);
	if (!cmd->args)
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
			break ;
		add_command(&head, &current, cmd);
		if (token && token->type == TOKEN_PIPE)
			token = token->next;
	}
	return (head);
}
