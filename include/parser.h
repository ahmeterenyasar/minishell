#ifndef PARSER_H
# define PARSER_H

# include "types.h"

/* Parser functions */
t_command					*parse_tokens(t_token *tokens);
t_redirect					*parse_redirection(t_token **token_ptr);
int							count_args(t_token *tokens);
t_command					*parse_command(t_token **token_ptr);
void						free_redirect(t_redirect *redirect);
void						free_command(t_command *cmd);
t_token 					*process_parser_token(t_token *token, t_command *cmd, int *arg_index);

/* Integrated interface */
t_command					*parse_input(const char *line, t_shell_data *shell);

/* Parser utility functions */
int							redir_is_redirection(t_token *token);
t_token						*handle_redirection_arg(t_token *token, t_command *cmd);
char						**allocate_args(int count);
void						fill_args(t_command *cmd, t_token **tokens, int arg_count);
int							is_valid_redir_token(t_token *token);
void						add_redirection(t_command *cmd, t_redirect *redir);
int							set_redirection_file(t_redirect *redir, t_token *token);
t_redirect					*create_redirection(t_token *token);

#endif