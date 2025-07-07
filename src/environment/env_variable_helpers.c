/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_variable_helpers.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:51:43 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 11:51:44 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*create_env_with_value(const char *name, const char *value)
{
	char	*new_var;

	new_var = malloc(ft_strlen(name) + ft_strlen(value) + 2);
	if (!new_var)
		return (NULL);
	ft_strcpy(new_var, name);
	ft_strcat(new_var, "=");
	ft_strcat(new_var, value);
	return (new_var);
}

char	*create_env_variable(const char *name, const char *value)
{
	char	*new_var;

	if (value)
		new_var = create_env_with_value(name, value);
	else
	{
		new_var = ft_strdup(name);
		if (!new_var)
			return (NULL);
	}
	return (new_var);
}

static char	**allocate_expanded_env(int count)
{
	char	**new_envp;

	new_envp = malloc(sizeof(char *) * (count + 2));
	return (new_envp);
}

static int	copy_env_vars_to_new(char **envp, char **new_envp, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		new_envp[i] = ft_strdup(envp[i]);
		if (!new_envp[i])
		{
			while (--i >= 0)
				free(new_envp[i]);
			free(new_envp);
			return (-1);
		}
		i++;
	}
	return (0);
}

char	**copy_existing_env_vars(char **envp, int count)
{
	char	**new_envp;

	new_envp = allocate_expanded_env(count);
	if (!new_envp)
		return (NULL);
	if (copy_env_vars_to_new(envp, new_envp, count) == -1)
		return (NULL);
	return (new_envp);
}
