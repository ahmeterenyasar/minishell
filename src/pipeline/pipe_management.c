/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:54:13 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:54:14 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	cleanup_pipes_on_error(int **pipes, int allocated_count)
{
	int	i;

	i = 0;
	while (i < allocated_count)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

static int	allocate_pipe_array(int **pipes, int pipe_count)
{
	int	i;

	i = 0;
	while (i < pipe_count)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i])
		{
			cleanup_pipes_on_error(pipes, i);
			return (-1);
		}
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			free(pipes[i]);
			cleanup_pipes_on_error(pipes, i);
			return (-1);
		}
		i++;
	}
	return (0);
}

int	**create_pipes(int pipe_count)
{
	int	**pipes;

	if (pipe_count <= 0)
		return (NULL);
	pipes = malloc(sizeof(int *) * pipe_count);
	if (!pipes)
		return (NULL);
	if (allocate_pipe_array(pipes, pipe_count) == -1)
		return (NULL);
	return (pipes);
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
