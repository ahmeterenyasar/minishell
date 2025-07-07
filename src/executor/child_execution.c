/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_execution.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:04 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:52:05 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_command_path(char *cmd_path, char *cmd_name, t_shell_data *shell,
		t_command *cmd)
{
	struct stat	path_stat;

	if (!cmd_path)
	{
		print_command_not_found_error(cmd_name);
		exit_with_cleanup(shell, cmd, 127);
	}
	if (stat(cmd_path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			print_directory_error(cmd_name);
			free(cmd_path);
			exit_with_cleanup(shell, cmd, 126);
		}
	}
	return (0);
}

void	handle_execve_error(char *cmd_path, char *cmd_name, t_shell_data *shell,
		t_command *cmd)
{
	if (errno == EACCES)
	{
		print_permission_error(cmd_name);
		free(cmd_path);
		free_envp(shell->envp);
		free(shell);
		free_command(cmd);
		exit(126);
	}
	perror("execve");
	free(cmd_path);
	free_envp(shell->envp);
	free(shell);
	free_command(cmd);
	exit(127);
}

void	execute_external_command(t_command *cmd, t_shell_data *shell)
{
	char	*cmd_path;
	char	**envp_backup;

	cmd_path = find_command_path(cmd->args[0], shell->envp);
	validate_command_path(cmd_path, cmd->args[0], shell, cmd);
	envp_backup = shell->envp;
	cleanup_shell_lines(shell);
	cleanup_readline_history();
	if (execve(cmd_path, cmd->args, envp_backup) == -1)
		handle_execve_error(cmd_path, cmd->args[0], shell, cmd);
}

void	execute_child_process(t_command *cmd, t_shell_data *shell)
{
	int	validation_result;

	reset_signals();
	if (setup_redirections(cmd->redirects) == -1)
		exit_with_cleanup(shell, cmd, 1);
	validation_result = validate_command_args(cmd);
	if (validation_result == 0)
		exit_with_cleanup(shell, cmd, 0);
	if (validation_result == -1)
	{
		print_command_not_found_error(cmd->args[0]);
		exit_with_cleanup(shell, cmd, 127);
	}
	if (is_builtin(cmd->args[0]))
		execute_builtin_command(cmd, shell);
	execute_external_command(cmd, shell);
}
