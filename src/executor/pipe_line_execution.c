#include "minishell.h"

/* Child process cleanup function to prevent memory leaks in valgrind */
static void	cleanup_child_inherited_memory(t_command *cmd, t_shell_data *shell)
{
	/* 
	 * In child processes, we need to clean up memory inherited from parent
	 * to avoid "still reachable" reports in valgrind. This function frees
	 * the command structure and shell data that were allocated during parsing.
	 */
	if (cmd)
		free_command(cmd);
	if (shell)
		free_shell_data(shell);
}

void	execute_pipeline_child(t_command *cmd, int cmd_index, int **pipes,
		int pipe_count, t_shell_data *shell)
{
	char	*cmd_path;

	reset_signals();
	setup_pipe_redirections(cmd_index, pipes, pipe_count);
	// Setup redirections AFTER setting up pipes
	// This ensures redirections override pipe settings when needed
	if (setup_redirections(cmd->redirects) == -1)
	{
		cleanup_child_inherited_memory(cmd, shell);
		exit(1);
	}
	if (!cmd->args || !cmd->args[0])
	{
		cleanup_child_inherited_memory(cmd, shell);
		exit(0);
	}
	// Check for empty command name
	if (cmd->args[0] && *cmd->args[0] == '\0')
	{
		write(STDERR_FILENO, "minishell: : command not found\n", 32);
		cleanup_child_inherited_memory(cmd, shell);
		exit(127);
	}
	if (is_builtin(cmd->args[0]))
	{
		int result = execute_builtin(cmd->args, shell);
		cleanup_child_inherited_memory(cmd, shell);
		exit(result);
	}
	cmd_path = find_command_path(cmd->args[0], shell->envp);
	if (!cmd_path)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		cleanup_child_inherited_memory(cmd, shell);
		exit(127);
	}
	// Before execve, cmd_path will be replaced by the new process image
	// So we don't need to free it as it will be cleaned up automatically
	if (execve(cmd_path, cmd->args, shell->envp) == -1)
	{
		if (errno == EACCES)
		{
			write(STDERR_FILENO, "minishell: ", 11);
			write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
			write(STDERR_FILENO, ": Permission denied\n", 20);
			free(cmd_path);
			cleanup_child_inherited_memory(cmd, shell);
			exit(126);
		}
		else
		{
			perror("execve");
			free(cmd_path);
			cleanup_child_inherited_memory(cmd, shell);
			exit(127);
		}
	}
	// If execve succeeds, this line is never reached
	// The new process image replaces this process
}

int	execute_pipeline(t_command *cmd, t_shell_data *shell)
{
	int			cmd_count;
	int			**pipes;
	pid_t		*pids;
	t_command	*current;
	int			i;
	int			result;

	cmd_count = count_commands(cmd);
	if (cmd_count == 1)
		return (execute_single_command(cmd, shell));
	pipes = create_pipes(cmd_count - 1);
	if (!pipes)
	{
		set_exit_status(shell, 1);
		return (1);
	}
	pids = malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		close_pipes(pipes, cmd_count - 1);
		set_exit_status(shell, 1);
		return (1);
	}
	current = cmd;
	i = 0;
	while (current && i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			close_pipes(pipes, cmd_count - 1);
			free(pids);
			set_exit_status(shell, 1);
			return (1);
		}
		if (pids[i] == 0)
		{
			cleanup_child_inherited_memory(current, shell);
			execute_pipeline_child(current, i, pipes, cmd_count - 1, shell);
		}
		current = current->next;
		i++;
	}
	close_all_pipes(pipes, cmd_count - 1);
	result = wait_for_pipeline(pids, cmd_count, shell);
	close_pipes(pipes, cmd_count - 1);
	free(pids);
	return (result);
}
