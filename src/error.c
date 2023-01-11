/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 18:32:39 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/10 19:06:16 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_error(char *error_message)
{
	ft_putstr_fd(error_message, 2);
}

// Error code = 0 means that i just want to print the error message and return 1
int	set_error(char *error_message, int error_code)
{
	(void)error_code;
	print_error(error_message);
	return (1);
}
