#include "minishell.h"

static int	handle_pipeline_fork_error(pid_t *pids, int started_children,
		int **pipes, int pipe_count, t_shell_data *shell)
{
	int	i;

	perror("fork");
	i = 0;
	while (i < started_children)
	{
		waitpid(pids[i], NULL, 0);
		i++;
	}
	close_all_pipes(pipes, pipe_count);
	close_pipes(pipes, pipe_count);
	free(pids);
	set_exit_status(shell, 1);
	return (1);
}

static int	initialize_pipeline_resources(int cmd_count, int ***pipes,
		pid_t **pids, t_shell_data *shell)
{
	*pipes = create_pipes(cmd_count - 1);
	if (!*pipes)
	{
		set_exit_status(shell, 1);
		return (1);
	}
	*pids = malloc(sizeof(pid_t) * cmd_count);
	if (!*pids)
	{
		close_pipes(*pipes, cmd_count - 1);
		set_exit_status(shell, 1);
		return (1);
	}
	return (0);
}

static int	execute_pipeline_children(t_command *cmd, int cmd_count,
		int **pipes, pid_t *pids, t_shell_data *shell)
{
	t_command	*current;
	int			i;

	current = cmd;
	i = 0;
	while (current && i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (handle_pipeline_fork_error(pids, i, pipes, cmd_count - 1,
					shell));
		if (pids[i] == 0)
		{
			execute_pipeline_child(current, i, pipes, cmd_count - 1, shell, cmd,
					pids);
			exit(1);
		}
		current = current->next;
		i++;
	}
	return (0);
}

int	execute_pipeline(t_command *cmd, t_shell_data *shell)
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	int		result;

	cmd_count = count_commands(cmd);
	if (cmd_count == 1)
		return (execute_single_command(cmd, shell));
	if (initialize_pipeline_resources(cmd_count, &pipes, &pids, shell) != 0)
		return (1);
	if (execute_pipeline_children(cmd, cmd_count, pipes, pids, shell) != 0)
		return (1);
	close_all_pipes(pipes, cmd_count - 1);
	result = wait_for_pipeline(pids, cmd_count, shell);
	close_pipes(pipes, cmd_count - 1);
	free(pids);
	return (result);
}
