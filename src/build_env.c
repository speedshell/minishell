#include "../includes/minishell.h"

char	**build_env(char **env)
{
	int		i;
	char	**line;

	i = 0;
	while (env[i])
		i++;
	line = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		line[i] = ft_strdup(env[i]);
		i++;
	}
	line[i] = NULL;
	return (line);
}
