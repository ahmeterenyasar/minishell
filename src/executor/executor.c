/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:35 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:52:36 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_command *cmd, t_shell_data *shell)
{
	int	result;
	int	heredoc_result;

	if (!cmd)
		return (0);
	reset_global_signal();
	heredoc_result = process_heredocs_with_signal_check(cmd, shell);
	if (heredoc_result > 0)
		return (heredoc_result);
	result = execute_pipeline(cmd, shell);
	if (!cmd->next)
	{
		heredoc_result = handle_global_signals(shell);
		if (heredoc_result > 0)
			return (heredoc_result);
	}
	if (shell->should_exit)
		return (result);
	return (result);
}
