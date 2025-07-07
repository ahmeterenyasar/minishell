/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayasar <ayasar@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 11:50:01 by ayasar            #+#    #+#             */
/*   Updated: 2025/07/07 16:12:15 by ayasar           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_cd(char **args, t_shell_data *shell)
{
	char		*old_pwd;
	t_cd_path	cd_info;

	if (validate_cd_args(args, shell))
		return (1);
	old_pwd = getcwd(NULL, 0);
	if (determine_cd_path(args, shell, old_pwd, &cd_info))
		return (1);
	return (change_to_directory(cd_info.path, old_pwd,
			cd_info.should_free_path, shell));
}
