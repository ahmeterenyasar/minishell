#include "minishell.h"

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

	// Check for empty command name
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
