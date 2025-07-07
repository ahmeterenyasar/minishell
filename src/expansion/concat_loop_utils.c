/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   concat_loop_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 16:30:00 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 17:27:46 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_init_data(t_expand_init_data *init_data,
						const char *str,
						t_shell_data *shell)
{
	init_data->str = str;
	init_data->shell = shell;
}
