/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/10 18:26:54 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/10 22:03:58 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <fcntl.h>

char	*get_line(void);
void	write_to_heredoc(char *text);
int		get_heredoc_fd(int *redirect);
char	*add_line_break(char *line);

int	here_doc(char *delimiter, int *redirect)
{
	char	*text;
	char	*line;
	char	*temp;

	text = NULL;
	temp = NULL;
	while (42)
	{
		line = get_line();
		if (line == NULL || (*line != '\n' && \
				ft_strncmp(delimiter, line, ft_strlen(line) - 1) == 0))
		{
			free(line);
			write_to_heredoc(text);
			return (get_heredoc_fd(redirect));
		}
		temp = text;
		if (text == NULL)
			text = ft_strdup(line);
		else
			text = ft_strjoin(text, line);
		free(temp);
		free(line);
	}
	return (0);
}

void	write_to_heredoc(char *text)
{
	int	heredoc_fd;
	int	text_size;

	if (text == NULL)
		return ;
	text_size = ft_strlen(text);
	heredoc_fd = open(".here_doc", O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	write(heredoc_fd, text, text_size);
	close(heredoc_fd);
	free(text);
}

int	get_heredoc_fd(int *redirect)
{
	int	fd;

	fd = open(".here_doc", O_RDONLY);
	redirect[0] = fd;
	if (fd == -1)
	{
		print_err_msg();
		return (-1);
	}
	return (HERE_DOC);
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
	line = add_line_break(line);
	add_history(line);
	return (line);
}
