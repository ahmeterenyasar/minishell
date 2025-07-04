#include "minishell.h"

/* Child process external command execution */

static void	cleanup_before_execve(t_shell_data *shell, int **pipes, 
		int pipe_count, pid_t *pids, t_command *cmd_list)
{
	if (shell && shell->current_lines)
	{
		free_str_array(shell->current_lines);
		shell->current_lines = NULL;
	}
	clear_history();
	rl_clear_history();
	rl_cleanup_after_signal();
	if (pipes && pipe_count > 0)
		close_pipes(pipes, pipe_count);
	if (pids)
		free(pids);
	if (cmd_list)
		free_command(cmd_list);
}

static void	handle_execve_permission_error(char *cmd_path, char *cmd_name, 
		char **args_backup, char **envp_backup, t_shell_data *shell)
{
	write(STDERR_FILENO, "minishell: ", 11);
	write(STDERR_FILENO, cmd_name, ft_strlen(cmd_name));
	write(STDERR_FILENO, ": Permission denied\n", 20);
	free(cmd_path);
	free_str_array(args_backup);
	free(cmd_name);
	free_envp(envp_backup);
	free(shell);
	exit(126);
}

static void	handle_execve_general_error(char *cmd_path, char *cmd_name, 
		char **args_backup, char **envp_backup, t_shell_data *shell)
{
	perror("execve");
	free(cmd_path);
	free_str_array(args_backup);
	free(cmd_name);
	free_envp(envp_backup);
	free(shell);
	exit(127);
}

static char	**duplicate_args_for_execve(t_command *cmd, t_shell_data *shell, 
		t_command *cmd_list, int **pipes, int pipe_count, pid_t *pids)
{
	char	**args_backup;
	int		args_count;
	int		i;

	args_count = 0;
	while (cmd->args[args_count])
		args_count++;
	args_backup = malloc(sizeof(char *) * (args_count + 1));
	if (!args_backup)
	{
		cleanup_pipeline_child_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(1);
	}
	i = 0;
	while (i < args_count)
	{
		args_backup[i] = ft_strdup(cmd->args[i]);
		if (!args_backup[i])
		{
			while (--i >= 0)
				free(args_backup[i]);
			free(args_backup);
			cleanup_pipeline_child_memory(shell, cmd_list, pipes, pipe_count, pids);
			exit(1);
		}
		i++;
	}
	args_backup[args_count] = NULL;
	return (args_backup);
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
	{
		if (errno == EACCES)
			handle_execve_permission_error(cmd_path, cmd_name_backup, 
				args_backup, envp_backup, shell);
		else
			handle_execve_general_error(cmd_path, cmd_name_backup, 
				args_backup, envp_backup, shell);
	}
}
