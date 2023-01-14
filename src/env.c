/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:12:46 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/14 11:56:07 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_env(char **env, char **args)
{
	int	i;

	i = 0;
	if (!env || !*env)
		return (0);
	if (args[1])
		return (set_error("Minishell: env: too many arguments\n", 1, NULL));
	while (env[i])
		printf("%s\n", env[i++]);
	g_exit_code = 0;
	return (0);
}
