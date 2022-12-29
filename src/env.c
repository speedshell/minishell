#include "../includes/minishell.h"

int	ft_env(char **env)
{
	int	i;

	i = 0;
	if (!env || !*env)
		return (0);
	while (env[i])
		printf("%s\n", env[i++]);
	return (0);
}
