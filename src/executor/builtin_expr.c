#include "minishell.h"

int     execute_expr(char **args, t_shell_data *shell)
{
    int     lhs;
    int     rhs;

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
    lhs = ft_atoi(args[1]);
    rhs = ft_atoi(args[3]);
    printf("%d\n", lhs + rhs);
    set_exit_status(shell, 0);
    return (0);
}
