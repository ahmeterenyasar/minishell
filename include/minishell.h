#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <ctype.h>
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

/* Error Codes */
# define SUCCESS 0
# define SYNTAX_ERROR 1
# define COMMAND_NOT_FOUND 127

/* Signal handler context */
# define INTERACTIVE_MODE 0
# define EXECUTING_MODE 1
# define HEREDOC_MODE 2

/* Token Types */
typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC
}							t_token_type;

/* Token Structure */
typedef struct s_token
{
	t_token_type			type;
	char					*value;
	int						expandable;
	struct s_token			*next;
}							t_token;

/* Redirection Structure */
typedef struct s_redirect
{
	t_token_type			type;
	char					*file;
	int						expand_heredoc;
	struct s_redirect		*next;
}							t_redirect;

/* Command Structure */
typedef struct s_command
{
	char					**args;
	t_redirect				*redirects;
	struct s_command		*next;
}							t_command;

/* Shell data structure to hold exit status and other shell state */
typedef struct s_shell_data
{
	int						last_exit_status;
	char					**envp;
}							t_shell_data;

/* Global variable for signal handling - ONLY allowed global */
extern sig_atomic_t			g_signal;

/* Signal handling functions */
void						setup_signals(int context);

/* Shell data management - replaces exit status list */
t_shell_data				*init_shell_data(char **envp);
void						set_exit_status(t_shell_data *shell, int status);
int							get_exit_status(t_shell_data *shell);
void						free_shell_data(t_shell_data *shell);

/* Tokenizer functions */
t_token						*tokenize(const char *input);
t_token						*create_token(t_token_type type, char *value,
								int expandable);
void						add_token(t_token **head, t_token *new_token);
int							handle_pipe(const char *input __attribute__((unused)),
								int i, t_token **head);
int							handle_redir_in(const char *input, int i,
								t_token **head);
int							handle_redir_out(const char *input, int i,
								t_token **head);
int							is_token_delimiter(char c);
int							is_operator_char(char c);
int							is_quote_char(char c);
int							extract_word_len(const char *input, int i);
int							add_word_token(const char *input, int i,
								t_token **head);
int							find_closing_quote(const char *input, int i,
								char quote_char);
int							handle_quotes(const char *input, int i,
								t_token **head);
void						free_tokens(t_token *tokens);

/* Syntax checker */
int							check_syntax(t_token *tokens);

/* Environment variable functions - now takes shell_data */
char						*expand_env_vars(const char *str, t_shell_data *shell);
char						*get_env_value(const char *name, t_shell_data *shell);
void						expand_tokens(t_token *tokens, t_shell_data *shell);
char						*get_exit_status_str(t_shell_data *shell);

/* Parser functions - now takes shell_data */
t_command					*parse_tokens(t_token *tokens);
t_redirect					*parse_redirection(t_token **token_ptr);
int							count_args(t_token *tokens);
t_command					*parse_command(t_token **token_ptr);
void						free_redirect(t_redirect *redirect);
void						free_command(t_command *cmd);

/* Integrated interface - now takes shell_data */
t_command					*parse_input(const char *line, t_shell_data *shell);

/* Heredoc functions - now takes shell_data */
int							process_heredoc(t_redirect *redirect, t_shell_data *shell);
int							process_all_heredocs(t_command *cmd, t_shell_data *shell);

/* Debug/utility functions */
void						print_tokens(t_token *tokens);
void						print_command(t_command *cmd);

int							ft_strcmp(const char *s1, const char *s2);
int							is_white_space(char c);
int							skip_white_space(const char *input, int i);

void						copy_quoted_text(const char *input, int start,
								int end, char *quoted_text, char quote_char);
int							create_quoted_token(char quote_char,
								char *quoted_text, t_token **head, int end);

int							is_all_whitespace(const char *str);

/*Helper functions to helper functions :)*/
void						print_error(const char *msg);
char						*int_to_string(int n);

#endif