/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/15 13:38:06 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/15 13:49:38 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	redirection_builtin_close(t_command *expr, int *std_backup)
{
	int	*redirection;

	redirection = expr->redirect;
	if (std_backup[0] != -1)
	{
		dup2(std_backup[0], STDIN_FILENO);
		close(redirection[0]);
	}
	if (std_backup[1] != -1)
	{
		dup2(std_backup[1], STDOUT_FILENO);
		close(redirection[1]);
	}
}

void	redirect_close(t_command *expr)
{
	int	*redirect;

	redirect = expr->redirect;
	if (redirect[0] != -1)
		close(redirect[0]);
	if (redirect[1] != -1)
		close (redirect[1]);
}
