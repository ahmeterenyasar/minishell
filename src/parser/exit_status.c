#include "minishell.h"

//aakyuz tavsiyesi ile yapıldı.
//exit statusları linked list ile tutuyoruz. son eklenen exit statusu alıyoruz.

t_exit_status	*g_exit_list = NULL;

void	add_exit_status(int status)
{
	t_exit_status	*new_status;

	new_status = malloc(sizeof(t_exit_status));
	if (!new_status)
		return ;
	new_status->status = status;
	new_status->next = g_exit_list;
	g_exit_list = new_status;
}

int	get_last_exit_status(void)
{
	if (!g_exit_list)
		return (0);
	return (g_exit_list->status);
}

void	free_exit_list(void)
{
	t_exit_status *current;
	t_exit_status *next;

	current = g_exit_list;
	while (current)
	{
		next = current->next;
		free(current);
		current = next;
	}
	g_exit_list = NULL;
}
