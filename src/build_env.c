/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_env.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:29:32 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/07 09:29:35 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**build_env(char **env)
{
	int		i;
	char	**line;

	i = 0;
	while (env[i])
		i++;
	line = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (env[i])
	{
		line[i] = ft_strdup(env[i]);
		i++;
	}
	line[i] = NULL;
	return (line);
}
