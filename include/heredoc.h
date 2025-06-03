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

#endif