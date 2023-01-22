/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:08 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/22 18:51:48 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>

char	*get_line(void);
void	write_to_heredoc(char *text);
int		get_heredoc_fd(int *redirect);
int		open_here_doc(char *here_doc_name);
char	*close_here_doc(char *here_doc_name, int here_doc_fd);
char	*delete_quotes(char *delimiter);
void	setup_vars(char **hd_filename, int *file_id, int *fd, char **delimiter);

char	*here_doc(char *delimiter)
{
	int				fd;
	static int		file_id;
	char			*line;
	char			*here_doc_name;

	line = NULL;
	setup_vars(&here_doc_name, &file_id, &fd, &delimiter);
	if (fd == -1)
		return (NULL);
	while (42)
	{
		line = get_line();
		if (line == NULL || \
				ft_strncmp(delimiter, line, ft_strlen(line) + 1) == 0)
		{
			free(delimiter);
			free(line);
			return (close_here_doc(here_doc_name, fd));
		}
		if (*line)
			ft_putendl_fd(line, fd);
		free(line);
	}
	return (0);
}

void	setup_vars(char **hd_filename, int *file_id, int *fd, char **delimiter)
{
	g_exit_code = -42;
	*hd_filename = gen_name(*file_id++);
	*fd = open_here_doc(*hd_filename);
	*delimiter = delete_quotes(*delimiter);
}

char	*delete_quotes(char *delimiter)
{
	char	*temp;

	delimiter = ft_strdup(delimiter);
	temp = quote_resolver(delimiter);
	if (temp != NULL)
	{
		free(delimiter);
		return (temp);
	}
	return (delimiter);
}

char	*get_line(void)
{
	char	*line;

	line = readline(">");
	if (g_exit_code == 130)
	{
		free(line);
		return (NULL);
	}
	return (line);
}
