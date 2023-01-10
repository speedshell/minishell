/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 14:30:54 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/10 15:13:29 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>
#include <unistd.h>

/*int	open_file_append(char *filename)
{
	
}*/

/*int	open_file_write()
{

}*/

int	file_open_read(char *filename, int *redirect)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		print_err_msg();	
		return (-1);
	}
	redirect[0] = fd;
	return (0);
}

int	file_open_write(char *filename, int *redirect)
{
	int	op_code;
	int	fd;

	fd = 0;
	op_code = access(filename, F_OK); 
	if (op_code != -1)
	{
		fd = open(filename, O_WRONLY | O_TRUNC);
	}
	else
	{
		fd = open(filename, O_CREAT | O_WRONLY\
			| S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}
	if (fd != -1)
	   redirect[1] = fd;
	else
		print_err_msg();
	return (fd);
}
