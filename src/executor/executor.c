#include "minishell.h"

/* Child process cleanup function to prevent memory leaks in valgrind */
static void	cleanup_child_inherited_memory(t_shell_data *shell)
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
		// Don't set to NULL as it's not needed in child about to exit
	}
	
	/* 
	 * For extra cleanliness, also free the environment array copy
	 * Again, this doesn't affect the parent due to copy-on-write
	 */
	if (shell && shell->envp)
	{
		free_envp(shell->envp);
	}
	
	/* Free the shell structure itself in child */
	if (shell)
	{
		free(shell);
	}
}

/* Process execution */
void	execute_child_process(t_command *cmd, t_shell_data *shell)
{
	char		*cmd_path;
	struct stat	path_stat;
	char		**envp_backup;

	reset_signals();
	if (setup_redirections(cmd->redirects) == -1)
	{
		cleanup_child_inherited_memory(shell);
		free_command(cmd);
		exit(1);
	}
	if (!cmd->args || !cmd->args[0])
	{
		cleanup_child_inherited_memory(shell);
		free_command(cmd);
		exit(0);
	}
	// Check for empty command name
	if (cmd->args[0] && *cmd->args[0] == '\0')
	{
		write(STDERR_FILENO, "minishell: : command not found\n", 32);
		cleanup_child_inherited_memory(shell);
		free_command(cmd);
		exit(127);
	}
	if (is_builtin(cmd->args[0]))
	{
		int result = execute_builtin(cmd->args, shell);
		cleanup_child_inherited_memory(shell);
		free_command(cmd);
		exit(result);
	}
	cmd_path = find_command_path(cmd->args[0], shell->envp);
	if (!cmd_path)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
		// If command contains '/', it's a path, so "No such file or directory"
		if (ft_strchr(cmd->args[0], '/'))
			write(STDERR_FILENO, ": No such file or directory\n", 28);
		else
			write(STDERR_FILENO, ": command not found\n", 20);
		cleanup_child_inherited_memory(shell);
		free_command(cmd);
		exit(127);
	}
	// Check if the path is a directory before calling execve
	if (stat(cmd_path, &path_stat) == 0)
	{
		if (S_ISDIR(path_stat.st_mode))
		{
			write(STDERR_FILENO, "minishell: ", 11);
			write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
			write(STDERR_FILENO, ": Is a directory\n", 17);
			free(cmd_path);
			cleanup_child_inherited_memory(shell);
			free_command(cmd);
			exit(126);
		}
	}
	
	// Backup envp and args before cleanup since we need them for execve
	envp_backup = shell->envp;
	
	// Clean up before execve (which replaces the process image)
	// But don't clean up envp and cmd yet since we need them for execve
	if (shell && shell->current_lines)
	{
		free_str_array(shell->current_lines);
		shell->current_lines = NULL;
	}
	
	// Clean up readline
	clear_history();
	rl_clear_history();
	rl_cleanup_after_signal();
	
	// execve will replace the entire process image if successful
	// If it fails, we need to clean up cmd_path before exit
	if (execve(cmd_path, cmd->args, envp_backup) == -1)
	{
		// execve failed - clean up everything
		if (errno == EACCES)
		{
			write(STDERR_FILENO, "minishell: ", 11);
			write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
			write(STDERR_FILENO, ": Permission denied\n", 20);
			free(cmd_path);
			free_envp(envp_backup);
			free(shell);
			free_command(cmd);
			exit(126);
		}
		else
		{
			perror("execve");
			free(cmd_path);
			free_envp(envp_backup);
			free(shell);
			free_command(cmd);
			exit(127);
		}
	}
	// This line should never be reached if execve succeeds
}

static int	handle_builtin_redirections(t_command *cmd, t_shell_data *shell)
{
	int	stdin_backup;
	int	stdout_backup;
	int	result;

	stdin_backup = dup(STDIN_FILENO);
	stdout_backup = dup(STDOUT_FILENO);
	if (stdin_backup == -1 || stdout_backup == -1)
	{
		if (stdin_backup != -1)
			close(stdin_backup);
		if (stdout_backup != -1)
			close(stdout_backup);
		set_exit_status(shell, 1);
		return (1);
	}
	if (setup_redirections(cmd->redirects) == -1)
	{
		dup2(stdin_backup, STDIN_FILENO);
		dup2(stdout_backup, STDOUT_FILENO);
		close(stdin_backup);
		close(stdout_backup);
		set_exit_status(shell, 1);
		return (1);
	}
	result = execute_builtin(cmd->args, shell);
	dup2(stdin_backup, STDIN_FILENO);
	dup2(stdout_backup, STDOUT_FILENO);
	close(stdin_backup);
	close(stdout_backup);
	return (result);
}

int	execute_single_command(t_command *cmd, t_shell_data *shell)
{
	pid_t	pid;
	int		status;
	int		result;

	if (!cmd->args || !cmd->args[0])
		return (0);
	// Check for empty command name (from quoted empty strings)
	if (cmd->args[0] && *cmd->args[0] == '\0')
	{
		write(STDERR_FILENO, "minishell: : command not found\n", 32);
		set_exit_status(shell, 127);
		return (127);
	}
	// Handle builtins with special redirection handling
	if (is_builtin(cmd->args[0]))
	{
		result = handle_builtin_redirections(cmd, shell);
		// Propagate the special exit code (-42) if returned
		if (result == -42)
			return (-42);
		return (result);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		set_exit_status(shell, 1);
		return (1);
	}
	if (pid == 0)
		execute_child_process(cmd, shell);
	waitpid(pid, &status, 0);
	
	// Handle signal interruption - these take priority over normal exit status
	if (g_signal == SIGINT)
	{
		set_exit_status(shell, 130);
		return (130);
	}
	else if (g_signal == SIGQUIT)
	{
		set_exit_status(shell, 131);
		return (131);
	}
	
	// Only process normal exit status if no signal was received
	if (WIFEXITED(status))
		set_exit_status(shell, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_exit_status(shell, 128 + WTERMSIG(status));
	return (get_exit_status(shell));
}

/* Main executor function */
int	execute_command(t_command *cmd, t_shell_data *shell)
{
	int	result;

	if (!cmd)
		return (0);
	// Process heredocs first
	if (process_all_heredocs(cmd, shell) != 0)
	{
		// If heredoc processing failed (including CTRL+C), return immediately
		if (g_signal == SIGINT)
		{
			set_exit_status(shell, 130);
			return (130);
		}
		set_exit_status(shell, 1);
		return (1);
	}
	// If CTRL+C was pressed during heredoc processing, don't execute command
	if (g_signal == SIGINT)
	{
		set_exit_status(shell, 130);
		return (130);
	}
	result = execute_pipeline(cmd, shell);
	// Propagate the special exit code (-42) if returned
	if (result == -42)
		return (-42);
	return (result);
}
