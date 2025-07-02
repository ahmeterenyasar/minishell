#include "minishell.h"

/* Child process cleanup function to prevent memory leaks in valgrind */
static void	cleanup_child_inherited_memory(t_shell_data *shell, t_command *cmd_list, 
		int **pipes, int pipe_count, pid_t *pids)
{
	/* 
	 * In child processes, we need to clean up memory inherited from parent
	 * to avoid "still reachable" reports in valgrind. 
	 * 
	 * Since fork() creates a copy-on-write memory space, freeing memory
	 * in the child doesn't affect the parent's memory.
	 */
	
	/* Clean up readline history inherited from parent */
	clear_history();
	rl_clear_history();
	rl_cleanup_after_signal();
	
	/* 
	 * Free the lines array to prevent "still reachable" in valgrind
	 * This is safe because:
	 * 1. fork() gives child its own memory space (copy-on-write)
	 * 2. Child process is about to exit anyway
	 * 3. Parent maintains its own copy of the data
	 */
	if (shell && shell->current_lines)
	{
		free_str_array(shell->current_lines);
		shell->current_lines = NULL;
	}
	
	/* 
	 * For extra cleanliness, also free the environment array copy
	 * Again, this doesn't affect the parent due to copy-on-write
	 */
	if (shell && shell->envp)
	{
		free_envp(shell->envp);
		shell->envp = NULL;
	}
	
	/* Free inherited pipeline structures */
	if (pipes && pipe_count > 0)
	{
		close_pipes(pipes, pipe_count);
	}
	
	if (pids)
	{
		free(pids);
	}
	
	/* Free the entire command list inherited from parent */
	if (cmd_list)
	{
		free_command(cmd_list);
	}
	
	/* Free the shell structure itself in child */
	if (shell)
	{
		free(shell);
	}
}

void	execute_pipeline_child(t_command *cmd, int cmd_index, int **pipes,
		int pipe_count, t_shell_data *shell, t_command *cmd_list, pid_t *pids)
{
	char	*cmd_path;
	char	**envp_backup;

	reset_signals();
	setup_pipe_redirections(cmd_index, pipes, pipe_count);
	// Setup redirections AFTER setting up pipes
	// This ensures redirections override pipe settings when needed
	if (setup_redirections(cmd->redirects) == -1)
	{
		cleanup_child_inherited_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(1);
	}
	if (!cmd->args || !cmd->args[0])
	{
		cleanup_child_inherited_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(0);
	}
	// Check for empty command name
	if (cmd->args[0] && *cmd->args[0] == '\0')
	{
		write(STDERR_FILENO, "minishell: : command not found\n", 32);
		cleanup_child_inherited_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(127);
	}
	if (is_builtin(cmd->args[0]))
	{
		int result = execute_builtin(cmd->args, shell);
		cleanup_child_inherited_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(result);
	}
	cmd_path = find_command_path(cmd->args[0], shell->envp);
	if (!cmd_path)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		cleanup_child_inherited_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(127);
	}
	
	// Backup both envp and cmd args before cleanup since we need them for execve
	envp_backup = shell->envp;
	
	// Manually duplicate the args array to avoid use-after-free
	int args_count = 0;
	while (cmd->args[args_count])
		args_count++;
	
	char **args_backup = malloc(sizeof(char *) * (args_count + 1));
	if (!args_backup)
	{
		cleanup_child_inherited_memory(shell, cmd_list, pipes, pipe_count, pids);
		exit(1);
	}
	
	int i = 0;
	while (i < args_count)
	{
		args_backup[i] = ft_strdup(cmd->args[i]);
		if (!args_backup[i])
		{
			// Cleanup on failure
			while (--i >= 0)
				free(args_backup[i]);
			free(args_backup);
			cleanup_child_inherited_memory(shell, cmd_list, pipes, pipe_count, pids);
			exit(1);
		}
		i++;
	}
	args_backup[args_count] = NULL;
	
	char *cmd_name_backup = ft_strdup(cmd->args[0]);  // Duplicate command name for error messages
	
	// Clean up before execve (which replaces the process image)
	// But don't clean up envp and args yet since we need them for execve
	if (shell && shell->current_lines)
	{
		free_str_array(shell->current_lines);
		shell->current_lines = NULL;
	}
	
	// Clean up readline
	clear_history();
	rl_clear_history();
	rl_cleanup_after_signal();
	
	// Clean up inherited pipeline structures before execve
	if (pipes && pipe_count > 0)
	{
		close_pipes(pipes, pipe_count);
	}
	if (pids)
	{
		free(pids);
	}
	if (cmd_list)
	{
		free_command(cmd_list);
	}
	
	// execve will replace the entire process image if successful
	if (execve(cmd_path, args_backup, envp_backup) == -1)
	{
		// execve failed - clean up everything including envp
		if (errno == EACCES)
		{
			write(STDERR_FILENO, "minishell: ", 11);
			write(STDERR_FILENO, cmd_name_backup, ft_strlen(cmd_name_backup));
			write(STDERR_FILENO, ": Permission denied\n", 20);
			free(cmd_path);
			free_str_array(args_backup);
			free(cmd_name_backup);
			free_envp(envp_backup);
			free(shell);
			exit(126);
		}
		else
		{
			perror("execve");
			free(cmd_path);
			free_str_array(args_backup);
			free(cmd_name_backup);
			free_envp(envp_backup);
			free(shell);
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
			// Wait for any children already started before cleaning up
			while (--i >= 0)
				waitpid(pids[i], NULL, 0);
			close_all_pipes(pipes, cmd_count - 1);
			close_pipes(pipes, cmd_count - 1);
			free(pids);
			set_exit_status(shell, 1);
			return (1);
		}
		if (pids[i] == 0)
		{
			execute_pipeline_child(current, i, pipes, cmd_count - 1, shell, cmd, pids);
			// Child should never reach here, but just in case
			exit(1);
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
