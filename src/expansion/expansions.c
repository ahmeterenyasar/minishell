/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:53:05 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/12 02:39:25 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_exit_status_str(t_shell_data *shell)
{
	int		status;
	char	*result;

	if (!shell)
		return (ft_strdup("0"));
	status = get_exit_status(shell);
	result = int_to_string(status);
	return (result);
}

char	*get_env_value(const char *name, t_shell_data *shell)
{
	int		i;
	int		len;
	char	**envp;

	if (!name || !shell || !shell->envp)
		return (ft_strdup(""));
	if (ft_strcmp(name, "?") == 0)
		return (get_exit_status_str(shell));
	len = ft_strlen(name);
	envp = shell->envp;
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (ft_strdup(envp[i] + len + 1));
		i++;
	}
	return (ft_strdup(""));
}

int	extract_env_name(const char *str, int i, char *name, int max_len)
{
	int	j;

	j = 0;
	if (str[i] == '?')
	{
		name[j++] = str[i++];
		name[j] = '\0';
		return (i);
	}
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_') && j < max_len - 1)
	{
		name[j++] = str[i++];
	}
	name[j] = '\0';
	return (i);
}
