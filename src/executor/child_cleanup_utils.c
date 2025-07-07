/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_cleanup_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:51:50 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:51:51 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup_child_readline(void)
{
	clear_history();
	rl_clear_history();
	rl_cleanup_after_signal();
}

static void	cleanup_child_shell_data(t_shell_data *shell)
{
	if (shell && shell->current_lines)
	{
		free_str_array(shell->current_lines);
		shell->current_lines = NULL;
	}
	if (shell && shell->envp)
	{
		free_envp(shell->envp);
		shell->envp = NULL;
	}
}

static void	cleanup_child_pipeline_data(int **pipes, int pipe_count,
		pid_t *pids, t_command *cmd_list)
{
	if (pipes && pipe_count > 0)
		close_pipes(pipes, pipe_count);
	if (pids)
		free(pids);
	if (cmd_list)
		free_command(cmd_list);
}

void	cleanup_pipeline_child_memory(t_shell_data *shell, t_command *cmd_list,
		int **pipes, int pipe_count, pid_t *pids)
{
	cleanup_child_readline();
	cleanup_child_shell_data(shell);
	cleanup_child_pipeline_data(pipes, pipe_count, pids, cmd_list);
	if (shell)
		free(shell);
}
