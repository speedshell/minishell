#include "../includes/minishell.h"

int valid_variable (char *c)
{
	int	i;

	i = -1;
	while (c[++i])
	{
		if (c[i] == '=' && i == 0)
			return (0) ;
		else if (c[i] == '=' && i != 1)
			break ;
		else if ((!ft_isalpha(c[i]) && !(c[i] == '_')) && i == 0)
        	return (0);
    	else if ((!ft_isalpha(c[i]) && !ft_isdigit(c[i]) && !(c[i] == '_')))
        	return (0);
	}
	return (1);
}

void	search_and_replace(char *variable, t_env *env, char *value)
{
    int     i;
    int     len;
	char	*temp;
	char	**new_env;

    i = -1;
    len = ft_strlen(variable);
	while (env->env[++i])
    {
        if (!ft_strncmp(variable, env->env[i], len))
        {
			temp = env->env[i];
			env->env[i] = ft_strjoin(variable, value);
			free(temp);
            return ;
        }
    }
	new_env = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (env->env[i])
	{
		new_env[i] = ft_strdup(env->env[i]);
		free(env->env[i++]);
	}
	free(env->env[i]);
	free(env->env);
	new_env[i++] = ft_strjoin(variable, value);
	new_env[i] = NULL;
	env->env = new_env;
    return ;
}

int ft_export(char **args, t_env *env)
{
	int		i;
	char	*variable;
	char	*value;
	int		j;

	j = 0;
	if (!args[1])
		return (ft_env(env->env));
	while (args[++j])
	{
		if (valid_variable(args[j]) && (ft_strchr(args[j], '=')))
		{
			i = 0;
			while (args[j][i++] != '=');
			variable = ft_substr(args[j], 0, i);
			value = ft_strdup(ft_strchr(args[j], '=') + 1);
			search_and_replace(variable, env, value);
			free(variable);
			free(value);
		}
		else
		{
			if (ft_strchr(args[j], '='))
			{
				i = 0;
				while (args[j][i++] != '=');
				variable = ft_substr(args[j], 0, i);
				printf("Minishell: export: `%s': not a valid identifier\n", variable);
				free(variable);
			}
			else
				printf("Minishell: export: `%s': not a valid identifier\n", args[j]);
		}
	}
	return (0);
}
