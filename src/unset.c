#include "../includes/minishell.h"

int	ft_unset(char **args, t_env *env)
{
	int		i;
	char	**new_env;
	int		j;
	char	*arg;

	i = 0;
	while (env->env[i])
		i++;
	if (!args[1])
		return (0);
	arg = ft_strjoin(args[1], "=");
	new_env = malloc(sizeof(char *) * (i));
	i = 0;
	j = 0;
	while (env->env[i])
	{
		if (ft_strncmp(arg, env->env[i], ft_strlen(arg)))
			new_env[j++] = ft_strdup(env->env[i]);
		free(env->env[i++]);
	}
	free(env->env);
	free(arg);
	new_env[j] = NULL;
	env->env = new_env;
	return (0);
}
