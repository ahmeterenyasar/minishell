#include "minishell.h"

void	copy_env_value(char *result, int *j, char *value)
{
    int	k;

    if (!value)
        return;
    
    k = 0;
    while (value[k] && *j < 4095)
    {
        result[(*j)++] = value[k++];
    }
    free(value);
}

static char	*expand_env_vars_in_redirects(const char *str, t_shell_data *shell)
{
    char	*result;
    int		i;
    int		j;
    char	var_name[256];
    char	*value;

    if (!str)
        return (ft_strdup(""));
    result = malloc(4096);
    if (!result)
        return (ft_strdup(""));
    i = 0;
    j = 0;
    while (str[i] && j < 4095)
    {
        if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) 
            || str[i + 1] == '_' || str[i + 1] == '?' || str[i + 1] == '$'))
        {
            i++;
            i = extract_env_name(str, i, var_name, sizeof(var_name));
            value = get_env_value(var_name, shell);
            copy_env_value(result, &j, value);
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    return (result);
}

char	*expand_env_vars(const char *str, t_shell_data *shell)
{
    char	*result;
    int		i;
    int		j;
    char	var_name[256];
    char	*value;

    if (!str)
        return (ft_strdup(""));
    result = malloc(4096);
    if (!result)
        return (ft_strdup(""));
    i = 0;
    j = 0;
    while (str[i] && j < 4095)
    {
        if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) 
            || str[i + 1] == '_' || str[i + 1] == '?' || str[i + 1] == '$'))
        {
            i++;
            i = extract_env_name(str, i, var_name, sizeof(var_name));
            value = get_env_value(var_name, shell);
            copy_env_value(result, &j, value);
        }
        else
            result[j++] = str[i++];
    }
    result[j] = '\0';
    return (result);
}

static void	expand_redirect_files(t_redirect *redirects, t_shell_data *shell)
{
    t_redirect	*current;
    char		*expanded;

    current = redirects;
    while (current)
    {
        if (current->file && current->type != TOKEN_HEREDOC)
        {
            expanded = expand_env_vars_in_redirects(current->file, shell);
            if (expanded)
            {
                free(current->file);
                current->file = expanded;
            }
        }
        current = current->next;
    }
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
            // Use the improved expansion function that handles concatenated vars correctly
            expanded = expand_concatenated_vars(current->value, shell);
            if (expanded)
            {
                free(current->value);
                current->value = expanded;
            }
        }
        current = current->next;
    }
}

void	expand_command_redirects(t_command *cmd, t_shell_data *shell)
{
    t_command	*current;

    current = cmd;
    while (current)
    {
        if (current->redirects)
            expand_redirect_files(current->redirects, shell);
        current = current->next;
    }
}

/* Advanced expansion function that handles concatenated tokens correctly */
char	*expand_concatenated_vars(const char *str, t_shell_data *shell)
{
	char	*result;
	int		i, j;
	char	var_name[256];
	char	*value;

	if (!str)
		return (ft_strdup(""));
	
	result = malloc(4096);
	if (!result)
		return (ft_strdup(""));
	
	i = 0;
	j = 0;
	
	// Skip leading whitespace first
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
	{
		result[j++] = str[i++];
	}
	
	while (str[i] && j < 4095)
	{
		if (str[i] == '$' && str[i + 1] && (ft_isalnum(str[i + 1]) 
			|| str[i + 1] == '_' || str[i + 1] == '?' || str[i + 1] == '$'))
		{
			// Found a variable - extract and expand it
			i++; // Skip the $
			
			// Extract variable name with proper boundary checking
			if (str[i] == '?' || str[i] == '$')
			{
				// Special single-character variables
				var_name[0] = str[i];
				var_name[1] = '\0';
				i++;
				
				// Get the value and expand it
				value = get_env_value(var_name, shell);
				if (value)
				{
					int k = 0;
					while (value[k] && j < 4095)
						result[j++] = value[k++];
					free(value);
				}
			}
			else
			{
				// Regular variable name - extract the full variable name first
				int name_len = 0;
				
				// Extract the full variable name
				while (str[i + name_len] && str[i + name_len] != '\x01' && (ft_isalnum(str[i + name_len]) || str[i + name_len] == '_') && name_len < 255)
				{
					name_len++;
				}
				
				if (name_len > 0)
				{
					// Create variable name
					ft_strncpy(var_name, str + i, name_len);
					var_name[name_len] = '\0';
					
					// Get the value (empty string for unset variables)
					value = get_env_value(var_name, shell);
					if (value)
					{
						// Copy the value (even if empty)
						int k = 0;
						while (value[k] && j < 4095)
							result[j++] = value[k++];
						free(value);
					}
					i += name_len;
				}
				else
				{
					// No valid variable name found - copy the $ and continue
					result[j++] = '$';
				}
			}
		}
		else if (str[i] == '\x01')
		{
			// Skip boundary markers - they're just for variable parsing
			i++;
		}
		else
		{
			// Regular character - copy as-is
			result[j++] = str[i++];
		}
	}
	result[j] = '\0';
	return (result);
}
