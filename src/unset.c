/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 19:10:02 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/22 20:18:49 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		is_valid_identifier(char *identifier);
char	**create_env(char **env);
char	**del_variable(char *var_name, char **old_env);

int	ft_unset(t_info *shell_data)
{
	char	**vars;

	vars = shell_data->cmd;
	vars++;
	if (!*vars)
		g_exit_code = 0;
	while (*vars)
	{
		if (!is_valid_identifier(*vars))
		{
			printf("Minishell: unset: `%s': not a valid identifier\n", *vars);
			vars++;
			g_exit_code = 1;
			continue ;
		}
		shell_data->env = del_variable(*vars, shell_data->env);
		vars++;
	}
	return (g_exit_code);
}

char	**del_variable(char *var_name, char **old_env)
{
	char		**new_env;
	int			i;
	int			j;

	if (!var_name || !old_env)
		return (NULL);
	i = 0;
	j = 0;
	new_env = create_env(old_env);
	while (old_env[i])
	{
		if (ft_strncmp(var_name, old_env[i], \
			ft_strchr(old_env[i], '=') - old_env[i]) != 0)
		{
			new_env[j++] = ft_strdup(old_env[i]);
		}
		free(old_env[i++]);
	}			
	free(old_env);
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
