/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:30:54 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/15 14:36:13 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <unistd.h>

int	file_open_write(char *filename, int *redirect, int mode);

int	file_open_read(char *filename, int *redirect)
{
	int	fd;

	fd = -1;
	if (access(filename, F_OK | R_OK) == -1)
	{
		g_exit_code = 1;
		print_err_msg1(filename);
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (redirect[0] != -1)
		close(redirect[0]);
	redirect[0] = fd;
	return (0);
}

int	file_open_write(char *filename, int *redirect, int flag)
{
	int	exists;
	int	fd;

	fd = 0;
	exists = access(filename, F_OK);
	if (exists != -1)
		fd = open(filename, O_WRONLY | flag);
	else
	{
		fd = open(filename, O_WRONLY | O_CREAT, \
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}
	if (fd != -1)
	{
		if (redirect[1] != -1)
			close(redirect[1]);
		redirect[1] = fd;
	}
	else
	{	
		g_exit_code = 1;
		print_err_msg1(filename);
	}
	return (fd);
}

void	redirect_setup(t_command *expr)
{
	if (expr->redirect[0] != -1)
		dup2(expr->redirect[0], STDIN_FILENO);
	if (expr->redirect[1] != -1)
		dup2(expr->redirect[1], STDOUT_FILENO);
}

void	redirection_builtin_setup(t_command *expr, int *std_backup)
{
	int	*redirection;

	redirection = NULL;
	if (!expr->has_redirect)
		return ;
	redirection = expr->redirect;
	if (std_backup[0] == -1)
		std_backup[0] = dup(STDIN_FILENO);
	if (std_backup[1] == -1)
		std_backup[1] = dup(STDOUT_FILENO);
	if (redirection[0] != -1)
	{
		dup2(redirection[0], STDIN_FILENO);
	}
	if (redirection[1] != -1)
	{
		dup2(redirection[1], STDOUT_FILENO);
	}
}
