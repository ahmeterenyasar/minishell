#include "minishell.h"

/* Functions for handling execve errors */

void	free_execve_backup_data(char *cmd_path, char *cmd_name, 
		char **args_backup, char **envp_backup, t_shell_data *shell)
{
	free(cmd_path);
	free_str_array(args_backup);
	free(cmd_name);
	free_envp(envp_backup);
	free(shell);
}

void	print_permission_denied_error(char *cmd_name)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
	write(STDERR_FILENO, ": Permission denied\n", 20);
}

void	handle_execve_permission_error(char *cmd_path, char *cmd_name, 
		char **args_backup, char **envp_backup, t_shell_data *shell)
{
	print_permission_denied_error(cmd_name);
	free_execve_backup_data(cmd_path, cmd_name, args_backup, envp_backup, shell);
	exit(126);
}

void	handle_execve_general_error(char *cmd_path, char *cmd_name, 
		char **args_backup, char **envp_backup, t_shell_data *shell)
{
	perror("execve");
	free_execve_backup_data(cmd_path, cmd_name, args_backup, envp_backup, shell);
	exit(127);
}
