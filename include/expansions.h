/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansions.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:56 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 13:52:01 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSIONS_H
# define EXPANSIONS_H

# include "types.h"

/* Utility functions */
void	copy_env_value(char *result, int *j, char *value);
int		check_dollar_expansion(const char *str, int i);
char	*allocate_result_buffer(void);
void	copy_regular_char(char *result, int *j, char c);

/* Environment variable expansion */
int		extract_env_name(const char *str, int i, char *name, int max_len);
char	*expand_env_vars(const char *str, t_shell_data *shell);
void	expand_loop(const char *str, char *result, char *var_name,
			t_shell_data *shell);
char	*get_env_value(const char *name, t_shell_data *shell);
char	*get_exit_status_str(t_shell_data *shell);

/* Concatenated variable expansion */
typedef struct s_var_expand_params
{
	const char		*str;
	int				*i;
	char			*var_name;
	t_shell_data	*shell;
	char			*result;
	int				*j;
}	t_var_expand_params;

void	handle_special_vars(t_var_expand_params *params);
void	handle_regular_vars(t_var_expand_params *params);
void	skip_leading_whitespace(t_var_expand_params *params);
char	*expand_concatenated_vars(const char *str, t_shell_data *shell);

/* Token expansion */
void	expand_tokens(t_token *tokens, t_shell_data *shell);

/* Redirect expansion */
void	expand_command_redirects(t_command *cmd, t_shell_data *shell);

#endif