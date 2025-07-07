/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_command_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:02 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 14:52:53 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_child_command(t_command *cmd, t_shell_data *shell,
		t_pipeline_context *ctx)
{
	if (!cmd->args || !cmd->args[0])
	{
		cleanup_pipeline_child_memory(shell, ctx);
		exit(0);
	}
	if (cmd->args[0] && (*cmd->args[0] == '\0'
			|| is_all_whitespace(cmd->args[0])))
	{
		print_command_not_found_error(cmd->args[0]);
		cleanup_pipeline_child_memory(shell, ctx);
		exit(127);
	}
	return (0);
}

static int	execute_child_builtin(t_command *cmd, t_shell_data *shell,
		t_pipeline_context *ctx)
{
	int	result;

	result = execute_builtin(cmd->args, shell);
	cleanup_pipeline_child_memory(shell, ctx);
	exit(result);
}

int	prepare_child_command(t_command *cmd, t_shell_data *shell,
		t_pipeline_context *ctx)
{
	validate_child_command(cmd, shell, ctx);
	if (is_builtin(cmd->args[0]))
		execute_child_builtin(cmd, shell, ctx);
	return (0);
}
