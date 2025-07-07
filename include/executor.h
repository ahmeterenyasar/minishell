/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:59:53 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 14:52:53 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "types.h"

/* Main executor functions */
int		execute_command(t_command *cmd, t_shell_data *shell);
int		execute_pipeline(t_command *cmd, t_shell_data *shell);
int		execute_single_command(t_command *cmd, t_shell_data *shell);

/* Path resolution functions */
char	*find_command_path(char *cmd, char **envp);

/* Environment utility functions */
char	*get_env_from_envp(char **envp, const char *name);

/* Process execution functions */
void	execute_child_process(t_command *cmd, t_shell_data *shell);
void	execute_pipeline_child(t_command *cmd, int cmd_index,
			t_pipeline_context *ctx, t_shell_data *shell);

/* Child cleanup functions */
void	cleanup_readline_history(void);
void	cleanup_shell_lines(t_shell_data *shell);
void	cleanup_shell_environment(t_shell_data *shell);
void	cleanup_child_inherited_memory(t_shell_data *shell);
void	exit_with_cleanup(t_shell_data *shell, t_command *cmd, int exit_code);

/* Command validation functions */
int		validate_command_args(t_command *cmd);
void	print_command_not_found_error(char *cmd_name);
void	print_directory_error(char *cmd_name);
void	print_permission_error(char *cmd_name);

/* Process signal handling functions */
int		handle_signal_termination(int status, t_shell_data *shell);
int		handle_global_signals(t_shell_data *shell);
int		check_heredoc_interruption(t_shell_data *shell);
void	reset_global_signal(void);

/* Builtin execution functions */
int		setup_builtin_redirections(t_command *cmd, int *stdin_backup,
			int *stdout_backup);
void	restore_builtin_redirections(int stdin_backup,
									int stdout_backup);
int		handle_builtin_redirections(t_command *cmd, t_shell_data *shell);
int		execute_builtin_command(t_command *cmd, t_shell_data *shell);

/* Child execution functions */
int		validate_command_path(char *cmd_path, char *cmd_name,
			t_shell_data *shell, t_command *cmd);
void	handle_execve_error(char *cmd_path, char *cmd_name, t_shell_data *shell,
			t_command *cmd);
void	execute_external_command(t_command *cmd, t_shell_data *shell);

/* Executor main functions */
int		handle_empty_command(t_command *cmd, t_shell_data *shell);
int		process_command_status(int status, t_shell_data *shell);
int	process_heredocs_with_signal_check(t_command *cmd,
										t_shell_data *shell);

/* Pipeline utility functions */
int		count_commands(t_command *cmd);
int		**create_pipes(int pipe_count);
void	close_pipes(int **pipes, int pipe_count);
void	close_all_pipes(int **pipes, int pipe_count);
void	setup_pipe_redirections(int cmd_index, int **pipes, int pipe_count);
int		wait_for_pipeline(pid_t *pids, int cmd_count, t_shell_data *shell);

/* Child cleanup functions */
void	cleanup_pipeline_child_memory(t_shell_data *shell,
									t_pipeline_context *ctx);

/* Child command preparation functions */
int		prepare_child_command(t_command *cmd, t_shell_data *shell,
			t_pipeline_context *ctx);

/* Child external command execution functions */
void	execute_external_child_command(t_command *cmd, char *cmd_path,
			t_shell_data *shell, t_pipeline_context *ctx);

/* Execve cleanup functions */
void	cleanup_shell_readline_data(t_shell_data *shell);
void	cleanup_pipeline_resources(int **pipes, int pipe_count, pid_t *pids);
void	cleanup_command_data(t_command *cmd_list);
void	cleanup_before_execve(t_shell_data *shell, int **pipes, int pipe_count,
			pid_t *pids, t_command *cmd_list);

/* Execve error handling functions */
void	free_execve_backup_data(char *cmd_path, char *cmd_name,
			char **args_backup, char **envp_backup, t_shell_data *shell);
void	print_permission_denied_error(char *cmd_name);
void	handle_execve_permission_error(char *cmd_path, char *cmd_name,
			char **args_backup, char **envp_backup, t_shell_data *shell);
void	handle_execve_general_error(char *cmd_path, char *cmd_name,
			char **args_backup, char **envp_backup, t_shell_data *shell);

/* Execve argument utility functions */
int		count_command_args(t_command *cmd);
char	**allocate_args_backup(int args_count, t_shell_data *shell,
			t_pipeline_context *ctx);
void	free_partial_args_backup(char **args_backup, int count);
void	copy_args_to_backup(t_command *cmd, char **args_backup, int args_count,
			t_shell_data *shell, t_pipeline_context *ctx);

/* Execve executor functions */
char	**duplicate_args_for_execve(t_command *cmd, t_shell_data *shell,
			t_pipeline_context *ctx);
void	handle_execve_error_cases(char *cmd_path, char *cmd_name_backup,
			char **args_backup, char **envp_backup, t_shell_data *shell);

#endif
