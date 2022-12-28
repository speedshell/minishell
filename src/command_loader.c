
#include "../includes/minishell.h"

char	*build_path(char *cmd_name, char *env);
void	free2d(void **matrix2d);

char	**parse_command(char *statement, char **env)
{
	char	**cmd_fields;
	char	*cmd_name;
	char	*fullpath_cmd;
	int		i;

	if (!statement || !*statement)
		return (NULL);
	cmd_fields = ft_split(statement, ' ');
	cmd_name = cmd_fields[0];
	if (access(cmd_name, X_OK) == 0)
		return (cmd_fields);
	i = 0;
	while (env[i] && ft_strncmp("PATH=", env[i], 5) != 0)
		i++;
	fullpath_cmd = build_path(cmd_name, env[i]);
	if (fullpath_cmd != NULL)
	{
		free(cmd_fields[0]);
		cmd_fields[0] = fullpath_cmd;
	}
	return (cmd_fields);
}

char	*build_path(char *cmd_name, char *path)
{
	char	**paths;
	char	*cmd_path;	
	char	*aux;	
	int		i;

	if (!cmd_name || !path)
		return (NULL);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i] != NULL)
	{
		aux = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(aux, cmd_name);
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

	if (!matrix2d || !*matrix2d)
		return ;
	i = 0;
	while (matrix2d[i] != NULL)
	{
		free(matrix2d[i]);
		i++;
	}
	free(matrix2d);
}
