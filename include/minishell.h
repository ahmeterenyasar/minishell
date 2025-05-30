#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
#include <sys/stat.h>
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
void						reset_signals(void);

/* Shell data management - replaces exit status list */
t_shell_data				*init_shell_data(char **envp);
void						set_exit_status(t_shell_data *shell, int status);
int							get_exit_status(t_shell_data *shell);
void						free_shell_data(t_shell_data *shell);

/* Tokenizer functions */
t_token						*tokenize(const char *input);
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

/* Token utility functions */
t_token						*create_token(t_token_type type, char *value,
								int expandable);
void						add_token(t_token **head, t_token *new_token);
char						*handle_newlines(const char *input);
int 						process_token(const char *input, int i, t_token **head);
int process_concatenated_word(const char *input, int i, t_token **head);
int	should_use_concatenation(const char *input, int i);

/* Input processing functions */
char	*convert_newlines(const char *input);
char	**split_commands_by_newlines(const char *input);

/* Word token utility functions */
int							extract_word_len(const char *input, int i);
void						copy_word_chars(const char *input, int i, char *word, int len);
int							add_word_token(const char *input, int i,
								t_token **head);
int							find_closing_quote(const char *input, int i,
								char quote_char);
void	free_str_array(char **arr);

/* Quote token utility functions */
void						copy_quoted_text(const char *input, int start,
								int end, char *quoted_text, char quote_char);
int							should_escape_char(char c);
int							create_quoted_token(char quote_char,
								char *quoted_text, t_token **head, int end);

/* Token validation functions */
int							is_token_delimiter(char c);
int							is_operator_char(char c);
int							is_quote_char(char c);
void						free_tokens(t_token *tokens);

/* Syntax checker */
int							check_syntax(t_token *tokens);

/* Expansion functions */
int							extract_env_name(const char *str, int i, char *name, int max_len);
void						copy_env_value(char *result, int *j, char *value);

/* Environment variable functions */
char						*expand_env_vars(const char *str, t_shell_data *shell);
char						*get_env_value(const char *name, t_shell_data *shell);
void						expand_tokens(t_token *tokens, t_shell_data *shell);
char						*get_exit_status_str(t_shell_data *shell);

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

/* Heredoc functions */
int							process_heredoc(t_redirect *redirect, t_shell_data *shell);
int							process_all_heredocs(t_command *cmd, t_shell_data *shell);

/* Debug/utility functions */
void						print_tokens(t_token *tokens);
void						print_command(t_command *cmd);

/* String utility functions - prefer ft_ versions for consistency */
int							ft_strcmp(const char *s1, const char *s2);
int							ft_strncmp(const char *s1, const char *s2, size_t n);
char						*ft_strdup(const char *s);
char						*ft_strjoin(const char *s1, const char *s2);
size_t						ft_strlen(const char *s);
char						*ft_strchr(const char *s, int c);
char						**ft_split(const char *s, char c);
int							ft_atoi(const char *str);
int							ft_isalnum(int c);
int							ft_isalpha(int c);
int							ft_isdigit(int c);
void						ft_putchar_fd(char c, int fd);

/* Whitespace functions */
int							is_white_space(char c);
int							skip_white_space(const char *input, int i);
int							is_all_whitespace(const char *str);

/* Parser utility functions */
int							redir_is_redirection(t_token *token);
t_token						*handle_redirection_arg(t_token *token, t_command *cmd);
char						**allocate_args(int count);
void						fill_args(t_command *cmd, t_token **tokens, int arg_count);
int							is_valid_redir_token(t_token *token);
void						add_redirection(t_command *cmd, t_redirect *redir);
int							set_redirection_file(t_redirect *redir, t_token *token);
t_redirect					*create_redirection(t_token *token);

/* Helper functions */
void						print_error(const char *msg);
char						*int_to_string(int n);

/* Executor functions */
int							execute_command(t_command *cmd, t_shell_data *shell);
int							execute_pipeline(t_command *cmd, t_shell_data *shell);
int							execute_single_command(t_command *cmd, t_shell_data *shell);

/* Built-in command functions */
int		execute_echo(char **args, t_shell_data *shell);
int		execute_pwd(t_shell_data *shell);
int		execute_cd(char **args, t_shell_data *shell);
int		execute_env(char **args, t_shell_data *shell);
int		execute_export(char **args, t_shell_data *shell);
int	set_env_variable(t_shell_data *shell, const char *name, const char *value);
int	find_env_var(char **envp, const char *name);
int		execute_unset(char **args, t_shell_data *shell);
int		execute_exit(char **args, t_shell_data *shell);
int		is_builtin(char *cmd);
int		execute_builtin(char **args, t_shell_data *shell);

/* Helper functions for builtins */
int		is_valid_n_flag(const char *arg);
int		is_valid_identifier(const char *name);
int		is_numeric_string(const char *str);
void	update_pwd_vars(t_shell_data *shell, char *old_pwd);
void	print_export_vars(char **envp);
void	print_env(char **envp);
void free_envp(char **envp);
char	**copy_envp(char **envp);

/* Path resolution functions */
char						*find_command_path(char *cmd, char **envp);
void						free_string_array(char **arr);

/* Redirection handling functions */
int							setup_input_redirection(t_redirect *redirect);
int							setup_output_redirection(t_redirect *redirect);
int							setup_redirections(t_redirect *redirects);

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

/* SSS */
void	expand_command_redirects(t_command *cmd, t_shell_data *shell);

/* heredoc_parser.c functions */
int		is_delimiter_quoted(char *delimiter);
int		setup_heredoc_file(char **filename);
int		validate_heredoc_redirect(t_redirect *redirect);
char	*remove_quotes_from_delimiter(char *delimiter);

/* heredoc_executor.c functions */
int		process_heredoc(t_redirect *redirect, t_shell_data *shell);

/* Additional functions for executor (if needed) */
int		process_all_heredocs(t_command *cmd, t_shell_data *shell);
int		process_cmd_heredocs(t_command *cmd, t_shell_data *shell);


#endif