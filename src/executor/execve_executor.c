#include "minishell.h"

/* Functions for the main execve execution logic */

char	**duplicate_args_for_execve(t_command *cmd, t_shell_data *shell, 
		t_command *cmd_list, int **pipes, int pipe_count, pid_t *pids)
{
	char	**args_backup;
	int		args_count;

	args_count = count_command_args(cmd);
	args_backup = allocate_args_backup(args_count, shell, cmd_list, pipes, 
			pipe_count, pids);
	copy_args_to_backup(cmd, args_backup, args_count, shell, cmd_list, pipes, 
			pipe_count, pids);
	return (args_backup);
}

void	handle_execve_error_cases(char *cmd_path, char *cmd_name_backup, 
		char **args_backup, char **envp_backup, t_shell_data *shell)
{
	if (errno == EACCES)
		handle_execve_permission_error(cmd_path, cmd_name_backup, 
			args_backup, envp_backup, shell);
	else
		handle_execve_general_error(cmd_path, cmd_name_backup, 
			args_backup, envp_backup, shell);
}

void	execute_external_child_command(t_command *cmd, char *cmd_path, 
		t_shell_data *shell, t_command *cmd_list, int **pipes, 
		int pipe_count, pid_t *pids)
{
	char	**args_backup;
	char	**envp_backup;
	char	*cmd_name_backup;

	envp_backup = shell->envp;
	args_backup = duplicate_args_for_execve(cmd, shell, cmd_list, pipes, 
			pipe_count, pids);
	cmd_name_backup = ft_strdup(cmd->args[0]);
	cleanup_before_execve(shell, pipes, pipe_count, pids, cmd_list);
	if (execve(cmd_path, args_backup, envp_backup) == -1)
		handle_execve_error_cases(cmd_path, cmd_name_backup, args_backup, 
			envp_backup, shell);
}
