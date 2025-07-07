/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_execution.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:09 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 12:03:48 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_builtin_redirections(t_command *cmd, int *stdin_backup,
		int *stdout_backup)
{
	*stdin_backup = dup(STDIN_FILENO);
	*stdout_backup = dup(STDOUT_FILENO);
	if (*stdin_backup == -1 || *stdout_backup == -1)
	{
		if (*stdin_backup != -1)
			close(*stdin_backup);
		if (*stdout_backup != -1)
			close(*stdout_backup);
		return (-1);
	}
	if (setup_redirections(cmd->redirects) == -1)
	{
		dup2(*stdin_backup, STDIN_FILENO);
		dup2(*stdout_backup, STDOUT_FILENO);
		close(*stdin_backup);
		close(*stdout_backup);
		return (-1);
	}
	return (0);
}

void	restore_builtin_redirections(int stdin_backup, int stdout_backup)
{
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
}

int	handle_builtin_redirections(t_command *cmd, t_shell_data *shell)
{
	int	stdin_backup;
	int	stdout_backup;
	int	result;

	if (setup_builtin_redirections(cmd, &stdin_backup, &stdout_backup) == -1)
	{
		set_exit_status(shell, 1);
		return (1);
	}
	result = execute_builtin(cmd->args, shell);
	restore_builtin_redirections(stdin_backup, stdout_backup);
	return (result);
}

int	execute_builtin_command(t_command *cmd, t_shell_data *shell)
{
	int	result;

	result = execute_builtin(cmd->args, shell);
	exit_with_cleanup(shell, cmd, result);
	return (result);
}
