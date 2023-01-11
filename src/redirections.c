/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:30:54 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/10 18:33:46 by lfarias-         ###   ########.fr       */
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
		print_err_msg();
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	redirect[0] = fd;
	return (0);
}

int	file_open_trunc(char *filename, int *redirect)
{
	int	fd;

	fd = file_open_write(filename, redirect, O_TRUNC);
	return (fd);
}

int	file_open_append(char *filename, int *redirect)
{
	int	fd;

	fd = file_open_write(filename, redirect, O_APPEND);
	return (fd);
}

int	file_open_write(char *filename, int *redirect, int flag)
{
	int	exists;
	int	fd;

	fd = 0;
	exists = access(filename, F_OK);
	if (exists != -1)
	{
		fd = open(filename, O_WRONLY | flag);
	}
	else
	{
		fd = open(filename, O_WRONLY | O_CREAT, \
			S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}
	if (fd != -1)
	   redirect[1] = fd;
	else
		print_err_msg();
	return (fd);	
}
