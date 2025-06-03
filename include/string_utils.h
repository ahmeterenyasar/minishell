#ifndef STRING_UTILS_H
# define STRING_UTILS_H

# include <stddef.h>

/* String utility functions - prefer ft_ versions for consistency */
char						*ft_strcpy(char *dest, const char *src);
char						*ft_strcat(char *dest, const char *src);
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
void						free_str_array(char **arr);

/* Whitespace functions */
int							is_white_space(char c);
int							skip_white_space(const char *input, int i);
int							is_all_whitespace(const char *str);

/* Helper functions */
void						print_error(const char *msg);
char						*int_to_string(int n);

#endif