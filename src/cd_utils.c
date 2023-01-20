/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <leofariasrj25@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 16:28:18 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/20 17:03:27 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	cd_arg_check(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	if (i > 2)
	{
		set_error("Minishell: cd: too many arguments\n", 1, NULL);
		return (i);
	}
	else if (i == 1)
		return (1);
	return (0);
}

/*
*	description: finds the index of env_name in env
*	return: if env_name exists on env, return 1, if not return -1
*/

int	find_env(char *env_name, char **env)
{
	int	i;
	int	env_name_len;

	env_name_len = ft_strlen(env_name);
	i = 0;
	while (env[i] && ft_strncmp(env_name, env[i], env_name_len) != 0)
		i++;
	if (env[i] != NULL)
		return (i);
	return (-1);
}
