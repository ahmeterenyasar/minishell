#include "minishell.h"

char	*get_exit_status_str(t_shell_data *shell)
{
	char	buffer[16];
	int		status;

	status = get_exit_status(shell);
	snprintf(buffer, sizeof(buffer), "%d", status);
	return (strdup(buffer));
}

char	*get_shell_pid(void)
{
	char	buffer[16];

	snprintf(buffer, sizeof(buffer), "%d", getpid());
	return (strdup(buffer));
}

char	*get_env_value(const char *name, t_shell_data *shell)
{
	int		i;
	int		len;
	char	**envp;

	if (!name || !shell || !shell->envp)
		return (NULL);
	if (strcmp(name, "?") == 0)
		return (get_exit_status_str(shell));
	if (strcmp(name, "$") == 0)
		return (get_shell_pid());
	len = strlen(name);
	envp = shell->envp;
	i = 0;
	while (envp[i])
	{
		if (strncmp(envp[i], name, len) == 0 && envp[i][len] == '=')
			return (strdup(envp[i] + len + 1));
		i++;
	}
	return (strdup(""));
}

static int	extract_env_name(const char *str, int i, char *name, int max_len)
{
	int	j;

	j = 0;
	// Handle special variables
	if (str[i] == '?' || str[i] == '$')
	{
		name[j++] = str[i++];
		name[j] = '\0';
		return (i);
	}
	// Handle regular variable names (alphanumeric + underscore)
	while (str[i] && (isalnum(str[i]) || str[i] == '_') && j < max_len - 1)
	{
		name[j++] = str[i++];
	}
	name[j] = '\0';
	return (i);
}

char	*expand_env_vars(const char *str, t_shell_data *shell)
{
	char	*result;
	int		i;
	int		j;
	char	var_name[256];
	char	*value;
	int		k;

	if (!str)
		return (NULL);
	result = malloc(4096);
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i] && j < 4095)
	{
		if (str[i] == '$' && str[i + 1] && (isalnum(str[i + 1]) 
			|| str[i + 1] == '_' || str[i + 1] == '?' || str[i + 1] == '$'))
		{
			i++;
			i = extract_env_name(str, i, var_name, sizeof(var_name));
			value = get_env_value(var_name, shell);
			if (value)
			{
				k = 0;
				while (value[k] && j < 4095)
					result[j++] = value[k++];
				free(value);
			}
		}
		else
		{
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}

void	expand_tokens(t_token *tokens, t_shell_data *shell)
{
	t_token *current;
	char *expanded;

	current = tokens;
	while (current)
	{
		if (current->type == TOKEN_WORD && current->expandable)
		{
			expanded = expand_env_vars(current->value, shell);
			if (expanded)
			{
				free(current->value);
				current->value = expanded;
			}
		}
		current = current->next;
	}
}
