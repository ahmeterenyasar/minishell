#include "minishell.h"

int	handle_export_assignment(char *arg, t_shell_data *shell)
{
	return (export_handle_assignment_processing(arg, shell));
}
