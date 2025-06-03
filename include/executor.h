#ifndef EXECUTOR_H
# define EXECUTOR_H

# include "types.h"

/* Executor functions */
int							execute_command(t_command *cmd, t_shell_data *shell);
int							execute_pipeline(t_command *cmd, t_shell_data *shell);
int							execute_single_command(t_command *cmd, t_shell_data *shell);

/* Path resolution functions */
char						*find_command_path(char *cmd, char **envp);

/* Process execution functions */
void						execute_child_process(t_command *cmd, t_shell_data *shell);
void						execute_pipeline_child(t_command *cmd, int cmd_index, 
                                int **pipes, int pipe_count, t_shell_data *shell);

/* Pipeline utility functions */
int							count_commands(t_command *cmd);
int							**create_pipes(int pipe_count);
void						close_pipes(int **pipes, int pipe_count);
void						close_all_pipes(int **pipes, int pipe_count);
void						setup_pipe_redirections(int cmd_index, int **pipes, 
                                int pipe_count);
int							wait_for_pipeline(pid_t *pids, int cmd_count, 
                                t_shell_data *shell);

#endif
