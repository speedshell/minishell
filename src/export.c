/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:28:50 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/14 16:19:11 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	{
		ft_putstr_fd("Minishell: export: `", 2);
		ft_putstr_fd(variable, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
	}
	free(variable);
	free(value);
}

int	ft_export(char **args, t_env *env)
{
	int		j;

	j = 0;
	if (!args[1])
		return (ft_env(env->env, args));
	while (args[++j])
	{
		if (valid_variable(args[j]) && (ft_strchr(args[j], '=')))
			extract(env, args[j], 0);
		else
		{
			if (ft_strchr(args[j], '='))
				extract(env, args[j], 1);
			else
			{
				ft_putstr_fd("Minishell: export: `", 2);
				ft_putstr_fd(args[j], 2);
				ft_putstr_fd("': not a valid identifier\n", 2);
			}
		}
	}
	return (0);
}
