/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/18 14:26:08 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/18 14:41:07 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>

char	*get_line(void);
void	write_to_heredoc(char *text);
int		get_heredoc_fd(int *redirect);
char	*add_line_break(char *line);
int		open_here_doc(char *here_doc_name);
char	*close_here_doc(char *here_doc_name, int here_doc_fd);

char	*here_doc(char *delimiter)
{
	int				fd;
	static int		file_id;
	char			*line;
	char			*here_doc_name;

	g_exit_code = 0;
	line = NULL;
	here_doc_name = gen_name(file_id++);
	fd = open_here_doc(here_doc_name);
	if (fd == -1)
		return (NULL);
	while (42)
	{
		line = get_line();
		if (line == NULL || (*line != '\n' && \
				ft_strncmp(delimiter, line, ft_strlen(line) - 1) == 0))
		{
			free(line);
			return (close_here_doc(here_doc_name, fd));
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	return (0);
}

char	*add_line_break(char *line)
{
	char	*temp;

	if (!line)
		return (NULL);
	if (line && *line == '\0')
		return (ft_strdup("\n"));
	temp = line;
	line = ft_strjoin(line, "\n");
	free(temp);
	return (line);
}

char	*get_line(void)
{
	char	*line;

	line = readline(">");
	if (g_exit_code == 130)
		return (NULL);
	line = add_line_break(line);
	add_history(line);
	return (line);
}

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
