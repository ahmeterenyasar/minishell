#include "minishell.h"

/* Process execution */
void	execute_child_process(t_command *cmd, t_shell_data *shell)
{
	char	*cmd_path;

	reset_signals();
	
	if (setup_redirections(cmd->redirects) == -1)
		exit(1);

	if (!cmd->args || !cmd->args[0])
		exit(0);

	// NEW: Check for empty command name
	if (cmd->args[0] && *cmd->args[0] == '\0')
	{
		write(STDERR_FILENO, "minishell: : command not found\n", 32);
		exit(127);
	}

	if (is_builtin(cmd->args[0]))
	{
		exit(execute_builtin(cmd->args, shell));
	}

	cmd_path = find_command_path(cmd->args[0], shell->envp);
	if (!cmd_path)
	{
		write(STDERR_FILENO, "minishell: ", 11);
		write(STDERR_FILENO, cmd->args[0], ft_strlen(cmd->args[0]));
		write(STDERR_FILENO, ": command not found\n", 20);
		exit(127);
	}

	if (execve(cmd_path, cmd->args, shell->envp) == -1)
	{
		perror("execve");
		free(cmd_path);
		exit(1);
	}
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
	
	// Handle signal interruption
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
	
	if (WIFEXITED(status))
		set_exit_status(shell, WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		set_exit_status(shell, 128 + WTERMSIG(status));
	
	return (get_exit_status(shell));
}

/* Main executor function */
int execute_command(t_command *cmd, t_shell_data *shell)
{
    int result;

    if (!cmd)
        return (0);

    if (process_all_heredocs(cmd, shell) != 0)
    {
        set_exit_status(shell, 1);
        return (1);
    }

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
