#include "minishell.h"

/* Pipeline child process main execution */

void	execute_pipeline_child(t_command *cmd, int cmd_index, int **pipes,
		int pipe_count, t_shell_data *shell, t_command *cmd_list, pid_t *pids)
{
	char	*cmd_path;

	reset_signals();
	setup_pipe_redirections(cmd_index, pipes, pipe_count);
	if (setup_redirections(cmd->redirects) == -1)
	{
		cleanup_pipeline_child_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(1);
	}
	prepare_child_command(cmd, shell, cmd_list, pipes, pipe_count, pids);
	cmd_path = find_command_path(cmd->args[0], shell->envp);
	if (!cmd_path)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		cleanup_pipeline_child_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(127);
	}
	execute_external_child_command(cmd, cmd_path, shell, cmd_list, 
		pipes, pipe_count, pids);
}
