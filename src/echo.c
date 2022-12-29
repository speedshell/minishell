/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 14:16:14 by lfarias-          #+#    #+#             */
/*   Updated: 2022/12/29 16:44:11 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_flag(char *flag);

/* 
 *	description: prints the argument on the STDOUT	
 *  usage: echo [-n] string1 string2 .... stringN
 *  options: -n [removes the trailing new line]
 *  return: 0 on success, 1 on failure [wrong args]
 */

int	ft_echo(char **args)
{
	int	is_flag_valid;
	int	i;

	i = 0;
	is_flag_valid = check_flag(args[i]);
	i++;
	if (is_flag_valid)
	{
		while (args[i] != NULL)
		{
			if (check_flag(args[i]) != 0 && is_flag_valid != 0)
				printf("%s ", args[i]);
			else
				is_flag_valid = 0;
			i++;
		}
	}
	else
	{
		while (args[i++] != NULL)
			printf("%s ", args[i]);
		printf("\n");
	}
	return (0);
}

int	check_flag(char *flag)
{
	int	i;

	if (!flag)
		return (0);
	i = 0;
	if (flag[i] == '-')
		i++;
	else
		return (0);
	while (flag[i] == 'n')
	{
		i++;
	}
	if (flag[i] != '\0')
		return (0);
	else
		return (1);
}
