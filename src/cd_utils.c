/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <leofariasrj25@gmail.com>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/19 16:28:18 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/19 16:34:48 by lfarias-         ###   ########.fr       */
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
