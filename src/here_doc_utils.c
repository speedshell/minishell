/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/22 18:44:24 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/22 18:51:40 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>

int	open_here_doc(char *here_doc_name)
{
	int	fd;

	fd = open(here_doc_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd == -1)
		print_err_msg();
	return (fd);
}

char	*close_here_doc(char *here_doc_name, int here_doc_fd)
{
	if (close(here_doc_fd) == -1)
		print_err_msg();
	if (g_exit_code == 130)
	{
		unlink(here_doc_name);
		free(here_doc_name);
		return (NULL);
	}
	return (here_doc_name);
}
