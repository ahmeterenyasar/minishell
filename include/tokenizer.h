#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "types.h"

/* Tokenizer functions */
t_token						*tokenize(const char *input);
void						add_token(t_token **head, t_token *new_token);
int							handle_pipe(const char *input __attribute__((unused)),
								int i, t_token **head);
int							handle_redir_in(const char *input, int i,
								t_token **head);
int							handle_redir_out(const char *input, int i,
								t_token **head);
int							handle_quotes(const char *input, int i,
								t_token **head);
void						free_tokens(t_token *tokens);

/* Token utility functions */
t_token						*create_token(t_token_type type, char *value,
								int expandable);
char						*handle_newlines(const char *input);
int 						process_token(const char *input, int i, t_token **head);
int							process_concatenated_word(const char *input, int i, t_token **head);
int							should_use_concatenation(const char *input, int i);

/* Input processing functions */
char						*convert_newlines(const char *input);
char						**split_commands_by_newlines(const char *input);

/* Word token utility functions */
int							extract_word_len(const char *input, int i);
void						copy_word_chars(const char *input, int i, char *word, int len);
int							add_word_token(const char *input, int i,
								t_token **head);
int							find_closing_quote(const char *input, int i,
								char quote_char);

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

/* Syntax checker */
int							check_syntax(t_token *tokens);

#endif