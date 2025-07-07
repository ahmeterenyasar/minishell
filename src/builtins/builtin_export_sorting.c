/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_sorting.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:27 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:50:28 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count])
		count++;
	return (count);
}

static char	**copy_env_array(char **envp, int count)
{
	char	**sorted;
	int		i;

	sorted = malloc(sizeof(char *) * (count + 1));
	if (!sorted)
		return (NULL);
	i = 0;
	while (i < count)
	{
		sorted[i] = envp[i];
		i++;
	}
	sorted[count] = NULL;
	return (sorted);
}

static void	swap_env_vars(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static void	bubble_sort_env(char **sorted, int count)
{
	int	i;
	int	j;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - 1 - i)
		{
			if (ft_strcmp(sorted[j], sorted[j + 1]) > 0)
				swap_env_vars(&sorted[j], &sorted[j + 1]);
			j++;
		}
		i++;
	}
}

char	**sort_envp_for_export(char **envp)
{
	char	**sorted;
	int		count;

	count = count_env_vars(envp);
	sorted = copy_env_array(envp, count);
	if (!sorted)
		return (NULL);
	bubble_sort_env(sorted, count);
	return (sorted);
}
