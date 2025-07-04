#include "minishell.h"

/* Pipeline execution */

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

	if (pipe_count <= 0)
		return (NULL);
	
	pipes = malloc(sizeof(int *) * pipe_count);
	if (!pipes)
		return (NULL);

	i = 0;
	while (i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			// Clean up previously allocated pipes
			while (--i >= 0)
			{
				close(pipes[i][0]);
				close(pipes[i][1]);
				free(pipes[i]);
			}
			free(pipes);
			return (NULL);
		}
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			// Clean up current pipe and previously allocated pipes
			free(pipes[i]);
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
		
		// For the last command in the pipeline, get its exit status
		// POSIX: pipeline exit status = exit status of last command
		if (i == cmd_count - 1)
		{
			if (WIFEXITED(status))
				last_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				int signal_num = WTERMSIG(status);
				if (signal_num == SIGINT)
					last_status = 130;
				else if (signal_num == SIGQUIT)
					last_status = 131;
				else
					last_status = 128 + signal_num;
			}
		}
		i++;
	}
	
	// Use the last command's exit status (POSIX pipeline behavior)
	set_exit_status(shell, last_status);
	return (last_status);
}
