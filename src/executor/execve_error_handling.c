/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve_error_handling.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:52:43 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 16:02:19 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_execve_backup_data(t_execve_data *execve_data, t_shell_data *shell)
{
	free(execve_data->cmd_path);
	free_str_array(execve_data->args_backup);
	free(execve_data->cmd_name_backup);
	free_envp(execve_data->envp_backup);
	free(shell);
}

void	print_permission_denied_error(char *cmd_name)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
	write(STDERR_FILENO, ": Permission denied\n", 20);
}

void	handle_execve_permission_error(t_execve_data *execve_data,
	t_shell_data *shell)
{
	print_permission_denied_error(execve_data->cmd_name_backup);
	free_execve_backup_data(execve_data, shell);
	exit(126);
}

void	handle_execve_general_error(t_execve_data *execve_data,
		t_shell_data *shell)
{
	perror("execve");
	free_execve_backup_data(execve_data, shell);
	exit(127);
}
