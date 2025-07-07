/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_cleanup.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:41 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:52:42 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_shell_readline_data(t_shell_data *shell)
{
	if (shell && shell->current_lines)
	{
		free_str_array(shell->current_lines);
		shell->current_lines = NULL;
	}
	clear_history();
	rl_clear_history();
}

void	cleanup_pipeline_resources(int **pipes, int pipe_count, pid_t *pids)
{
	if (pipes && pipe_count > 0)
		close_pipes(pipes, pipe_count);
	if (pids)
		free(pids);
}

void	cleanup_command_data(t_command *cmd_list)
{
	if (cmd_list)
		free_command(cmd_list);
}

void	cleanup_before_execve(t_shell_data *shell, int **pipes, int pipe_count,
		pid_t *pids, t_command *cmd_list)
{
	cleanup_shell_readline_data(shell);
	cleanup_pipeline_resources(pipes, pipe_count, pids);
	cleanup_command_data(cmd_list);
}
