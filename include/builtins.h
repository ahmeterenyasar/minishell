/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:51 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 16:12:15 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "types.h"

/* CD path structure */
typedef struct s_cd_path
{
	char	*path;
	int		should_free_path;
}			t_cd_path;

/* Built-in command functions */
int		execute_echo(char **args, t_shell_data *shell);
int		execute_pwd(t_shell_data *shell);
int		execute_cd(char **args, t_shell_data *shell);
int		execute_env(char **args, t_shell_data *shell);
int		execute_export(char **args, t_shell_data *shell);
int		set_env_variable(t_shell_data *shell, const char *name,
			const char *value);
int		find_env_var(char **envp, const char *name);
int		execute_unset(char **args, t_shell_data *shell);
int		execute_expr(char **args, t_shell_data *shell);
int		execute_exit(char **args, t_shell_data *shell);
int		is_builtin(char *cmd);
int		execute_builtin(char **args, t_shell_data *shell);

/* Export-related functions */
int		export_without_value(t_shell_data *shell, const char *name);
int		handle_export_assignment(char *arg, t_shell_data *shell);
char	**sort_envp_for_export(char **envp);

/* Export error handling functions */
int		export_validate_and_report_error(char *arg);
int		export_report_memory_error(void);

/* Export validation utility functions */
char	*export_duplicate_argument(char *arg);
int		export_validate_identifier(char *name, char *arg);
int		export_check_assignment_format(char *arg);

/* Export assignment processing functions */
int		export_handle_assignment_processing(char *arg, t_shell_data *shell);

/* Helper functions for builtins */
int		is_valid_n_flag(const char *arg);
int		is_valid_identifier(const char *name);
int		is_numeric_string(const char *str);
void	update_pwd_vars(t_shell_data *shell, char *old_pwd);
void	print_export_vars(char **envp);
void	print_env(char **envp);
void	free_envp(char **envp);
char	**copy_envp(char **envp);
char	**add_env_var(char **envp, const char *name, const char *value);
int		update_env_var(char **envp, const char *name, const char *value,
			int index);
char	**copy_existing_env_vars(char **envp, int count);
char	*create_env_variable(const char *name, const char *value);
char	**create_env_without_var(char **envp, int skip_index);
int		remove_env_variable(t_shell_data *shell, const char *name);
int		process_unset_args(t_shell_data *shell, char **args);

/* CD command helper functions */
int		validate_cd_args(char **args, t_shell_data *shell);
char	*get_home_path(t_shell_data *shell, char *old_pwd);
char	*get_oldpwd_path(t_shell_data *shell, char *old_pwd);
int		change_to_directory(char *path, char *old_pwd, int should_free_path,
			t_shell_data *shell);
int		determine_cd_path(char **args, t_shell_data *shell, char *old_pwd,
			t_cd_path *cd_info);

#endif