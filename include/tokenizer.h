/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:00:19 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 14:17:03 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKENIZER_H
# define TOKENIZER_H

# include "types.h"

/* Tokenizer functions */
t_token	*tokenize(const char *input);
void	add_token(t_token **head, t_token *new_token);
int		handle_pipe(const char *input __attribute__((unused)), int i,
			t_token **head);
int		handle_redir_in(const char *input, int i, t_token **head);
int		handle_redir_out(const char *input, int i, t_token **head);
int		handle_quotes(const char *input, int i, t_token **head);
void	free_tokens(t_token *tokens);

/* Token utility functions */
t_token	*create_token(t_token_type type, char *value, int expandable);
t_token	*create_quoted_token_with_flag(t_token_type type, char *value,
			int expandable, int quoted);
int		has_unclosed_quotes(const char *input);
int		process_token(const char *input, int i, t_token **head);
int		process_single_token(const char *processed_input, int i, t_token **head);
int		process_concatenated_word(const char *input, int i, t_token **head);
int		should_use_concatenation(const char *input, int i);

/* Input processing functions */
char	*convert_newlines(const char *input);
char	**split_commands_by_newlines(const char *input);

/* Word token utility functions */
int		extract_word_len(const char *input, int i);
void	copy_word_chars(const char *input, int i, char *word, int len);
int		add_word_token(const char *input, int i, t_token **head);
int		find_closing_quote(const char *input, int i, char quote_char);

/* Quote token utility functions */
void	copy_quoted_text(const char *input, t_quote_pos pos, char *quoted_text);
int		should_escape_char(char c);
int		create_quoted_token(char quote_char, char *quoted_text, t_token **head,
			int end);

/* Token validation functions */
int		is_token_delimiter(char c);
int		is_operator_char(char c);
int		is_quote_char(char c);
int		skip_unquoted_text(const char *input, int pos);

/* Syntax checker */
int		check_syntax(t_token *tokens);

/* Tokenizer helper functions - split into multiple files */
int		process_quoted_part(const char *input, int *i,
			t_word_parts_params *params);
int		process_unquoted_part(const char *input, int *i,
			t_word_parts_params *params);
int		check_boundary_markers_needed(char **word_parts, int part_count);
char	*concatenate_word_parts(char **word_parts, int part_count,
			int need_boundary_markers);
int		handle_single_quoted_token(const char *input, int start_pos,
			char **word_parts, t_token **head);
int		calculate_total_length(char **word_parts, int part_count);
int		finalize_concatenated_word(char **word_parts, int part_count,
			t_finalize_params params, t_token **head);

/* Word part utility functions */
void	free_word_parts(char **word_parts, int count);
int		handle_word_allocation_error(char **word_parts, int part_count);
int		allocate_word_part(char **word_parts, int part_count, int length);
void	finalize_word_part(int *part_count);

#endif