/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:14:28 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/19 00:06:51 by lfarias-         ###   ########.fr       */
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

void	print_err_str(char *err_msg, int error_code)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(err_msg, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (error_code == 127)
		ft_putendl_fd("command not found", STDERR_FILENO);
	else
		ft_putendl_fd(strerror(errno), STDERR_FILENO);
}

void	print_syntax_err(t_token *token)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", \
		STDERR_FILENO);
	if (!token)
		ft_putstr_fd("newline", STDERR_FILENO);
	else
		ft_putstr_fd(token->value, STDERR_FILENO);
	ft_putendl_fd("'", STDERR_FILENO);
}

void	print_quote_err(int quote)
{
	ft_putstr_fd("minishell: syntax error: unclosed `", STDERR_FILENO);
	if (quote == 1)
		ft_putendl_fd("''", STDERR_FILENO);
	else if (quote == 2)
		ft_putendl_fd("\"'", STDERR_FILENO);
}
