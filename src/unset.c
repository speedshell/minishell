/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 19:10:02 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/06 20:14:08 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		is_valid_identifier(char *identifier);
char	**create_env(char **env);
char	**del_variable(char *var_name, char **old_env);

int	ft_unset(char **args, t_env *env)
{
	char	**vars;

	vars = args;
	while (*vars)
	{
		if (!is_valid_identifier(*vars))
		{
			vars++;
			continue ;
		}
		env->env = del_variable(*vars, env->env);
		vars++;
	}
	return (0);
}

char	**del_variable(char *var_name, char **old_env)
{
	char	**new_env;
	int		i;
	int		j;

	if (!var_name || !old_env)
		return (NULL);
	i = 0;
	j = 0;
	new_env = create_env(old_env);
	var_name = ft_strjoin(var_name, "=");
	while (old_env[i])
	{
		if (ft_strncmp(var_name, old_env[i], ft_strlen(var_name)))
			new_env[j++] = ft_strdup(old_env[i]);
		free(old_env[i++]);
	}			
	free(old_env);
	free(var_name);
	new_env[j] = NULL;
	return (new_env);
}

int	is_valid_identifier(char *identifier)
{
	int	i;

	i = 0;
	if (!(ft_isalpha(identifier[i]) || identifier[i] == '_'))
		return (0);
	i++;
	while (identifier[i])
	{
		if (ft_isalnum(identifier[i]) || identifier[i] == '_')
			i++;
		else
			return (0);
	}
	return (1);
}

char	**create_env(char **env)
{
	int		i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 1));
	if (!new_env)
		return (NULL);
	return (new_env);
}
