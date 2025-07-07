/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_processor.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:07 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 12:06:16 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern sig_atomic_t	g_signal;

static int	execute_parsed_command(t_command *cmd, t_shell_data *shell)
{
	setup_signals(EXECUTING_MODE);
	execute_command(cmd, shell);
	setup_signals(INTERACTIVE_MODE);
	if (shell->should_exit)
		return (1);
	return (0);
}

static int	handle_parsing_failure(t_shell_data *shell)
{
	if (g_signal == SIGINT)
		handle_sigint_in_parsing(shell);
	return (1);
}

int	process_single_command(char *line, t_shell_data *shell)
{
	t_command	*cmd;
	int			result;

	cmd = parse_input(line, shell);
	if (cmd)
	{
		result = execute_parsed_command(cmd, shell);
		free_command(cmd);
		return (result);
	}
	else
	{
		return (handle_parsing_failure(shell));
	}
}

void	process_multiple_commands(char **lines, t_shell_data *shell)
{
	int	i;
	int	result;

	i = 0;
	while (lines[i])
	{
		if (*lines[i])
		{
			result = process_single_command(lines[i], shell);
			if (result == 1)
				return ;
		}
		i++;
	}
}
