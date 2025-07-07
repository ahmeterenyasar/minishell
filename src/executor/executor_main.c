/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_main.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:14 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 12:06:33 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_empty_command(t_command *cmd, t_shell_data *shell)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (cmd->args[0] && (*cmd->args[0] == '\0'
			|| is_all_whitespace(cmd->args[0])))
	{
		print_command_not_found_error(cmd->args[0]);
		set_exit_status(shell, 127);
		return (127);
	}
	return (-1);
}

int	process_command_status(int status, t_shell_data *shell)
{
	int	signal_result;

	signal_result = handle_signal_termination(status, shell);
	if (signal_result > 0)
		return (signal_result);
	if (WIFEXITED(status))
		set_exit_status(shell, WEXITSTATUS(status));
	return (get_exit_status(shell));
}

int	execute_single_command(t_command *cmd, t_shell_data *shell)
{
	pid_t	pid;
	int		status;
	int		empty_result;

	empty_result = handle_empty_command(cmd, shell);
	if (empty_result >= 0)
		return (empty_result);
	if (is_builtin(cmd->args[0]))
		return (handle_builtin_redirections(cmd, shell));
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		set_exit_status(shell, 1);
		return (1);
	}
	if (pid == 0)
		execute_child_process(cmd, shell);
	waitpid(pid, &status, 0);
	return (process_command_status(status, shell));
}

int	process_heredocs_with_signal_check(t_command *cmd, t_shell_data *shell)
{
	int	signal_result;

	if (process_all_heredocs(cmd, shell) != 0)
	{
		signal_result = check_heredoc_interruption(shell);
		if (signal_result > 0)
			return (signal_result);
		set_exit_status(shell, 1);
		return (1);
	}
	return (check_heredoc_interruption(shell));
}
