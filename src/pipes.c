/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 18:20:24 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/10 19:06:34 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pipes_setup(t_command *expr)
{
	if (expr->has_pipe)
	{
		close(expr->out_pipe[0]);
		dup2(expr->out_pipe[1], STDOUT_FILENO);
	}
	if (expr->in_pipe[0] != -1)
	{
		dup2(expr->in_pipe[0], STDIN_FILENO);
	}
}

void	pipes_close(t_command *expr)
{
	if (expr->in_pipe[0] != -1)
		close(expr->in_pipe[0]);
	if (expr->has_pipe)
		close(expr->out_pipe[1]);
}

void	pipes_builtin_setup(t_command *expr, int *std_backup)
{
	if (expr->has_pipe)
	{
		std_backup[0] = dup(STDIN_FILENO);
		std_backup[1] = dup(STDOUT_FILENO);
		dup2(expr->out_pipe[1], STDOUT_FILENO);
	}
}

void	pipes_builtin_close(t_command *expr, int *std_backup)
{
	if (expr->has_pipe)
	{
		close(expr->out_pipe[1]);
		dup2(std_backup[0], 0);
		dup2(std_backup[1], 1);
	}
}
