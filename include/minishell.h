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

/* Exit status tracking */
typedef struct s_exit_status
{
	int						status;
	struct s_exit_status	*next;
}							t_exit_status;

/* Global variable for signal handling */
extern sig_atomic_t			g_signal;
extern t_exit_status		*g_exit_list;

/* Signal handling functions */
void						setup_signals(int context);

/* Exit status management */
void						add_exit_status(int status);
int							get_last_exit_status(void);
void						free_exit_list(void);

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

/* Environment variable functions */
char						*expand_env_vars(const char *str, char **envp);
char						*get_env_value(const char *name, char **envp);
void						expand_tokens(t_token *tokens, char **envp);
char						*get_exit_status(void);

/* Parser functions */
t_command					*parse_tokens(t_token *tokens);
t_redirect					*parse_redirection(t_token **token_ptr);
int							count_args(t_token *tokens);
t_command					*parse_command(t_token **token_ptr);
void						free_redirect(t_redirect *redirect);
void						free_command(t_command *cmd);

/* Integrated interface */
t_command					*parse_input(const char *line, char **envp);

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

#endif