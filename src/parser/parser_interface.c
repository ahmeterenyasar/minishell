/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_interface.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:53:31 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:53:32 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		set_exit_status(shell, 2);
		return (NULL);
	}
	expand_tokens(tokens, shell);
	cmd = parse_tokens(tokens);
	free_tokens(tokens);
	tokens = NULL;
	if (cmd)
	{
		expand_command_redirects(cmd, shell);
	}
	return (cmd);
}
