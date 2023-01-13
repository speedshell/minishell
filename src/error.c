/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 18:32:39 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/12 19:49:19 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_error(char *error_message)
{
	ft_putstr_fd(error_message, 2);
}

// Error code = 0 means that i just want to print the error message and return 1
int	set_error(char *error_message, int error_code, char **args)
{
	g_exit_code = error_code;
	if (!args)
		print_error(error_message);
	else if (!ft_strncmp("cd", args[0], 2))
	{
		print_error(error_message);
		ft_putstr_fd(args[1], 2);
		print_error(": ");
		ft_putendl_fd(strerror(errno), 2);
	}
	return (g_exit_code);
}
