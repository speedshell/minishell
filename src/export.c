/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 22:07:25 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/07 00:21:07 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	valid_variable(char *c)
{
	int	i;

	i = -1;
	while (c[++i])
	{
		if (c[i] == '=' && i == 0)
			return (0);
		else if (c[i] == '=' && i != 0)
			break ;
		else if ((!ft_isalpha(c[i]) && !(c[i] == '_')) && i == 0)
			return (0);
		else if ((!ft_isalpha(c[i]) && !ft_isdigit(c[i]) && !(c[i] == '_')))
			return (0);
	}
	return (1);
}

void	replace(int i, t_env *env, char *variable, char *value)
{
	char	**new_env;

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

void	search_and_replace(char *variable, t_env *env, char *value)
{
	int		i;
	int		len;
	char	*temp;

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
	replace(i, env, variable, value);
}

void	extract(t_env *env, char *args, int flag)
{
	int		i;
	char	*variable;
	char	*value;

	i = 0;
	while (args[i] != '=')
		i++;
	variable = ft_substr(args, 0, i + 1);
	value = ft_strdup(ft_strchr(args, '=') + 1);
	if (flag == 0)
		search_and_replace(variable, env, value);
	if (flag == 1)
		printf("Minishell: export: `%s': not a valid identifier\n", variable);
	free(variable);
	free(value);
}

int	ft_export(char **args, t_env *env)
{
	int		j;

	j = 0;
	if (!args[1])
		return (ft_env(env->env));
	while (args[++j])
	{
		if (valid_variable(args[j]) && (ft_strchr(args[j], '=')))
			extract(env, args[j], 0);
		else
		{
			if (ft_strchr(args[j], '='))
				extract(env, args[j], 1);
			else
				printf("Minishell: export: `%s':"
					" not a valid identifier\n", args[j]);
		}
	}
	return (0);
}
