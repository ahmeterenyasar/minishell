#include "minishell.h"

/* Pipeline execution */
int	count_commands(t_command *cmd)
{
	int	count;

	count = 0;
	while (cmd)
	{
		count++;
		cmd = cmd->next;
	}
	return (count);
}

void	close_pipes(int **pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		if (pipes[i])
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			free(pipes[i]);
		}
		i++;
	}
	free(pipes);
}

int	**create_pipes(int pipe_count)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) * pipe_count);
	if (!pipes)
		return (NULL);

	i = 0;
	while (i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			free(pipes);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	setup_pipe_redirections(int cmd_index, int **pipes, int pipe_count)
{
	if (cmd_index > 0)
	{
		dup2(pipes[cmd_index - 1][0], STDIN_FILENO);
	}
	
	if (cmd_index < pipe_count)
	{
		dup2(pipes[cmd_index][1], STDOUT_FILENO);
	}

	close_all_pipes(pipes, pipe_count);
}

void	close_all_pipes(int **pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	execute_pipeline_child(t_command *cmd, int cmd_index, int **pipes,
							int pipe_count, t_shell_data *shell)
{
	char *cmd_path;

	reset_signals();
	setup_pipe_redirections(cmd_index, pipes, pipe_count);
	
	// Setup redirections AFTER setting up pipes
	// This ensures redirections override pipe settings when needed
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

int	wait_for_pipeline(pid_t *pids, int cmd_count, t_shell_data *shell)
{
	int		status;
	int		i;
	int		last_status;

	last_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)  // Only care about last command's exit status
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				last_status = 128 + WTERMSIG(status);
		}
		i++;
	}
	
	// Handle global signals
	if (g_signal == SIGINT)
		last_status = 130;
	else if (g_signal == SIGQUIT)
		last_status = 131;
	
	set_exit_status(shell, last_status);
	return (last_status);
}

int	execute_pipeline(t_command *cmd, t_shell_data *shell)
{
	int			cmd_count;
	int			**pipes;
	pid_t		*pids;
	t_command	*current;
	int			i;

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
			execute_pipeline_child(current, i, pipes, cmd_count - 1, shell);
		}
		
		current = current->next;
		i++;
	}

	close_all_pipes(pipes, cmd_count - 1);
	int result = wait_for_pipeline(pids, cmd_count, shell);
	
	close_pipes(pipes, cmd_count - 1);
	free(pids);
	return (result);
}