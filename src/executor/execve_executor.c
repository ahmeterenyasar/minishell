/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_executor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:46 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 16:07:01 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**duplicate_args_for_execve(t_command *cmd, t_shell_data *shell,
		t_pipeline_context *ctx)
{
	char					**args_backup;
	int						args_count;
	t_args_backup_context	backup_ctx;

	backup_ctx.shell = shell;
	backup_ctx.ctx = ctx;
	args_count = count_command_args(cmd);
	args_backup = allocate_args_backup(args_count, &backup_ctx);
	copy_args_to_backup(cmd, args_backup, args_count, &backup_ctx);
	return (args_backup);
}

void	handle_execve_error_cases(t_execve_data *execve_data,
								t_shell_data *shell)
{
	if (errno == EACCES)
		handle_execve_permission_error(execve_data, shell);
	else
		handle_execve_general_error(execve_data, shell);
}

void	execute_external_child_command(t_command *cmd, char *cmd_path,
		t_shell_data *shell, t_pipeline_context *ctx)
{
	t_execve_data	execve_data;

	execve_data.envp_backup = shell->envp;
	execve_data.args_backup = duplicate_args_for_execve(cmd, shell, ctx);
	execve_data.cmd_name_backup = ft_strdup(cmd->args[0]);
	execve_data.cmd_path = cmd_path;
	cleanup_before_execve(shell, ctx);
	if (execve(cmd_path, execve_data.args_backup, execve_data.envp_backup)
		== -1)
		handle_execve_error_cases(&execve_data, shell);
}
