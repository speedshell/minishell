/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:14:28 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/15 14:34:22 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "errno.h"

/* 
*	description: prints a formatted error message to STDERROR
*	return value: the errno associated with that error message
*/

int	print_err_msg(void)
{
	char	*error_string;

	error_string = ft_strjoin("minishell: ", strerror(errno));
	if (!error_string)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (errno);
	}
	ft_putendl_fd(error_string, 2);
	free(error_string);
	return (errno);
}

void	print_err_msg1(char *err_msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(strerror(errno), STDERR_FILENO);
}
