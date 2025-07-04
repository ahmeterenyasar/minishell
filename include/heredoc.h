#ifndef HEREDOC_H
# define HEREDOC_H

# include "types.h"

/* Heredoc functions */
int							process_heredoc(t_redirect *redirect, t_shell_data *shell);
int							process_all_heredocs(t_command *cmd, t_shell_data *shell);
int							process_cmd_heredocs(t_command *cmd, t_shell_data *shell);

/* heredoc_parser.c functions */
int							is_delimiter_quoted(char *delimiter);
int							setup_heredoc_file(char **filename);
int							validate_heredoc_redirect(t_redirect *redirect);
char						*remove_quotes_from_delimiter(char *delimiter);

/* Heredoc reading and processing functions */
int							read_heredoc_content(int fd, const char *delimiter, int expand,
								t_shell_data *shell);
int							read_heredoc_loop(int fd, const char *delimiter, int expand,
								t_shell_data *shell, int original_stdin, char *clean_delimiter);
int							check_signal_interruption(char *line, int original_stdin, 
								char *clean_delimiter);
int							handle_eof_condition(char *line);
int							check_delimiter_match(char *line, char *clean_delimiter);
int							process_heredoc_line(int fd, char *line, int expand,
								t_shell_data *shell);
int							handle_heredoc_failure(char *filename, int result);
void						finalize_heredoc_success(t_redirect *redirect, char *filename);
void						cleanup_heredoc_reading(int original_stdin, char *clean_delimiter);
int							write_heredoc_line(int fd, char *line, int expand,
								t_shell_data *shell);
int							setup_heredoc_reading(int *original_stdin);

#endif