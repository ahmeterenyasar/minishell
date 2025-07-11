/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_expr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:50 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/12 02:40:10 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	validate_expr_args_and_operator(char **args, t_shell_data *shell)
{
	if (!args[1] || !args[2] || !args[3] || args[4])
	{
		write(STDERR_FILENO, "expr: usage: expr operand1 + operand2\n", 38);
		set_exit_status(shell, 1);
		return (1);
	}
	if (ft_strcmp(args[2], "+") != 0)
	{
		write(STDERR_FILENO, "expr: only + operator supported\n", 32);
		set_exit_status(shell, 1);
		return (1);
	}
	return (0);
}

int	validate_integer_operand(char *operand, t_shell_data *shell)
{
	if (!is_numeric_string(operand))
	{
		write(STDERR_FILENO, "expr: ", 6);
		write(STDERR_FILENO, operand, ft_strlen(operand));
		write(STDERR_FILENO, ": not a valid integer\n", 22);
		set_exit_status(shell, 1);
		return (1);
	}
	return (0);
}

static void	output_expr_result(int result)
{
	char	*result_str;

	result_str = ft_itoa(result);
	if (result_str)
	{
		write(STDOUT_FILENO, result_str, ft_strlen(result_str));
		write(STDOUT_FILENO, "\n", 1);
		free(result_str);
	}
}

static int	calculate_addition(char **args)
{
	int	lhs;
	int	rhs;

	lhs = ft_atoi(args[1]);
	rhs = ft_atoi(args[3]);
	return (lhs + rhs);
}

int	execute_expr(char **args, t_shell_data *shell)
{
	int	result;

	if (validate_expr_args_and_operator(args, shell))
		return (1);
	if (validate_expr_operands(args, shell))
		return (1);
	result = calculate_addition(args);
	output_expr_result(result);
	set_exit_status(shell, 0);
	return (0);
}
