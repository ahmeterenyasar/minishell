/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:00:15 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 12:00:16 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRING_UTILS_H
# define STRING_UTILS_H

# include <stddef.h>

/* String utility functions not available in libft */
char	*ft_strcpy(char *dest, const char *src);
char	*ft_strcat(char *dest, const char *src);
int		ft_strcmp(const char *s1, const char *s2);

/* Whitespace functions */
int		is_white_space(char c);
int		skip_white_space(const char *input, int i);

/* Helper functions */
void	print_error(const char *msg);
char	*int_to_string(int n);

#endif