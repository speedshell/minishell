/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_pathfinder.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:24:36 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/18 14:45:14 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*build_cmd_path(char *cmd_name, char *env);
void	*clean_cmd_path(char **paths, char *aux);
int		check_state(char *ment);

char	*command_find_path(char *statement, char **env, int *builtin)
{
	char	*cmd_name;
	char	*fullpath_cmd;
	int		i;

	if (check_state(statement) == 0)
		return (NULL);
	cmd_name = statement;
	*builtin = is_builtin(cmd_name);
	if (*builtin != -1)
		return (cmd_name);
	if (ft_strncmp(cmd_name, "./", 2) == 0)
		return (cmd_name);
	if (access(cmd_name, F_OK | X_OK) == 0)
		return (cmd_name);
	i = 0;
	while (env[i] && ft_strncmp("PATH=", env[i], 5) != 0)
		i++;
	fullpath_cmd = build_cmd_path(cmd_name, env[i]);
	if (fullpath_cmd != NULL)
	{
		free(cmd_name);
		cmd_name = fullpath_cmd;
	}
	return (cmd_name);
}

char	*build_cmd_path(char *cmd_name, char *path)
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
			return (clean_cmd_path(paths, aux));
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

void	*clean_cmd_path(char **paths, char *aux)
{
	free2d((void **) paths);
	free(aux);
	return (NULL);
}

int	check_state(char *ment)
{
	if (!ment)
		return (0);
	if (!*ment)
	{
		ft_putendl_fd("minishell: :command not found", STDOUT_FILENO);
		g_exit_code = 127;
		return (0);
	}
	return (1);
}
