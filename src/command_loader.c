/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_loader.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:24:36 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/06 15:24:38 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*build_path(char *cmd_name, char *env);
void	free2d(void **matrix2d);
void	*clean_buildpath(char **paths, char *aux);

char	*parse_command(char *statement, char **env)
{
	char	*cmd_name;
	char	*fullpath_cmd;
	int		i;

	cmd_name = statement;
	if (is_builtin(cmd_name) != -1)
		return (cmd_name);
	if (access(cmd_name, X_OK) == 0)
		return (cmd_name);
	i = 0;
	while (env[i] && ft_strncmp("PATH=", env[i], 5) != 0)
		i++;
	fullpath_cmd = build_path(cmd_name, env[i]);
	if (fullpath_cmd != NULL)
	{
		free(cmd_name);
		cmd_name = fullpath_cmd;
	}
	return (cmd_name);
}

char	*build_path(char *cmd_name, char *path)
{
	char	**paths;
	char	*cmd_path;	
	char	*aux;	
	int		i;

	paths = ft_split(path, ':');
	if (paths == NULL)
		return (NULL);
	i = 0;
	while (paths[i] != NULL)
	{
		aux = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(aux, cmd_name);
		if (aux == NULL || cmd_path == NULL)
			return (clean_buildpath(paths, aux));
		free(aux);
		if (access(cmd_path, X_OK) == 0)
			break ;
		free(cmd_path);
		i++;
	}
	if (paths[i] == NULL)
		cmd_path = NULL;
	free2d((void **) paths);
	return (cmd_path);
}

void	free2d(void **matrix2d)
{
	int	i;

	if (!matrix2d)
		return ;
	i = 0;
	while (matrix2d[i] != NULL)
	{
		free(matrix2d[i]);
		i++;
	}
	free(matrix2d);
}

void	*clean_buildpath(char **paths, char *aux)
{
	free2d((void **) paths);
	free(aux);
	return (NULL);
}
