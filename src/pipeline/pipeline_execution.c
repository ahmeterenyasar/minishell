#include "minishell.h"

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

static int	get_signal_exit_status(int signal_num)
{
	if (signal_num == SIGINT)
		return (130);
	if (signal_num == SIGQUIT)
		return (131);
	return (128 + signal_num);
}

static int	get_process_exit_status(int status)
{
	int	signal_num;

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		signal_num = WTERMSIG(status);
		return (get_signal_exit_status(signal_num));
	}
	return (0);
}

int	wait_for_pipeline(pid_t *pids, int cmd_count, t_shell_data *shell)
{
	int	status;
	int	i;
	int	last_status;

	last_status = 0;
	i = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		if (i == cmd_count - 1)
			last_status = get_process_exit_status(status);
		i++;
	}
	set_exit_status(shell, last_status);
	return (last_status);
}
