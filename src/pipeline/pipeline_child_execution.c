/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_child_execution.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:54:15 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 14:52:53 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_pipeline_child(t_command *cmd, int cmd_index,
		t_pipeline_context *ctx, t_shell_data *shell)
{
	char	*cmd_path;

	reset_signals();
	setup_pipe_redirections(cmd_index, ctx->pipes, ctx->pipe_count);
	if (setup_redirections(cmd->redirects) == -1)
	{
		cleanup_pipeline_child_memory(shell, ctx);
		exit(1);
	}
	prepare_child_command(cmd, shell, ctx);
	cmd_path = find_command_path(cmd->args[0], shell->envp);
	if (!cmd_path)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		cleanup_pipeline_child_memory(shell, ctx);
		exit(127);
	}
	execute_external_child_command(cmd, cmd_path, shell, ctx);
}
