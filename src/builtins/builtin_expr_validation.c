/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_expr_validation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 02:28:14 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/12 02:28:17 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	validate_expr_operands(char **args, t_shell_data *shell)
{
	if (validate_integer_operand(args[1], shell))
		return (1);
	if (validate_integer_operand(args[3], shell))
		return (1);
	return (0);
}
