#include "minishell.h"

static int	validate_child_command(t_command *cmd, t_shell_data *shell,
		t_command *cmd_list, int **pipes, int pipe_count, pid_t *pids)
{
	if (!cmd->args || !cmd->args[0])
	{
		cleanup_pipeline_child_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(0);
	}
	if (cmd->args[0] && *cmd->args[0] == '\0')
	{
		write(STDERR_FILENO, "minishell: : command not found\n", 32);
		cleanup_pipeline_child_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(127);
	}
	return (0);
}

static int	execute_child_builtin(t_command *cmd, t_shell_data *shell,
		t_command *cmd_list, int **pipes, int pipe_count, pid_t *pids)
{
	int	result;

	result = execute_builtin(cmd->args, shell);
	cleanup_pipeline_child_memory(shell, cmd_list, pipes, pipe_count, pids);
	exit(result);
}

int	prepare_child_command(t_command *cmd, t_shell_data *shell,
		t_command *cmd_list, int **pipes, int pipe_count, pid_t *pids)
{
	validate_child_command(cmd, shell, cmd_list, pipes, pipe_count, pids);
	if (is_builtin(cmd->args[0]))
		execute_child_builtin(cmd, shell, cmd_list, pipes, pipe_count, pids);
	return (0);
}
