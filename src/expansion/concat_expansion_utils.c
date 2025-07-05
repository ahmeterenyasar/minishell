#include "minishell.h"

void	skip_leading_whitespace(const char *str, int *i, char *result, int *j)
{
	while (str[*i] && (str[*i] == ' ' || str[*i] == '\t'))
	{
		copy_regular_char(result, j, str[*i]);
		(*i)++;
	}
}

void	handle_special_vars(const char *str, int *i, char *var_name,
		t_shell_data *shell, char *result, int *j)
{
	char	*value;
	int		k;

	var_name[0] = str[*i];
	var_name[1] = '\0';
	(*i)++;
	value = get_env_value(var_name, shell);
	if (value)
	{
		k = 0;
		while (value[k] && *j < 4095)
			result[(*j)++] = value[k++];
		free(value);
	}
}

static int	extract_var_name_len(const char *str, int i)
{
	int	name_len;

	name_len = 0;
	while (str[i + name_len] && str[i + name_len] != '\x01')
	{
		if (!ft_isalnum(str[i + name_len]) && str[i + name_len] != '_')
			break ;
		if (name_len >= 255)
			break ;
		name_len++;
	}
	return (name_len);
}

void	handle_regular_vars(const char *str, int *i, char *var_name,
		t_shell_data *shell, char *result, int *j)
{
	int		name_len;
	char	*value;
	int		k;

	name_len = extract_var_name_len(str, *i);
	if (name_len > 0)
	{
		ft_strncpy(var_name, str + *i, name_len);
		var_name[name_len] = '\0';
		value = get_env_value(var_name, shell);
		if (value)
		{
			k = 0;
			while (value[k] && *j < 4095)
				result[(*j)++] = value[k++];
			free(value);
		}
		*i += name_len;
	}
	else
	{
		copy_regular_char(result, j, '$');
	}
}
