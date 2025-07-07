/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_cleanup_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:51:50 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 14:52:53 by ayasar           ###   ########.fr       */
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

static void	cleanup_child_pipeline_data(t_pipeline_context *ctx)
{
	if (ctx->pipes && ctx->pipe_count > 0)
		close_pipes(ctx->pipes, ctx->pipe_count);
	if (ctx->pids)
		free(ctx->pids);
	if (ctx->cmd_list)
		free_command(ctx->cmd_list);
}

void	cleanup_pipeline_child_memory(t_shell_data *shell,
		t_pipeline_context *ctx)
{
	cleanup_child_readline();
	cleanup_child_shell_data(shell);
	cleanup_child_pipeline_data(ctx);
	if (shell)
		free(shell);
}
