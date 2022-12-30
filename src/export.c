#include "../includes/minishell.h"

int	ft_export(char **args, t_env *env)
{
	int		i;
	char	**new_env;

	i = 0;
	if (!args[1])
		return (ft_env(env->env));
	while (env->env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env->env[i])
	{
		new_env[i] = ft_strdup(env->env[i]);
		free(env->env[i++]);
	}
	free(env->env[i]);
	free(env->env);
	if (args[1])
		if (!ft_isdigit(args[1][0]))
			new_env[i++] = ft_strdup(args[1]);
	new_env[i] = NULL;
	env->env = new_env;
	i = 0;
	return (0);
}	
