/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/29 17:47:37 by lfarias-          #+#    #+#             */
  <<<<<<< 66-centralizar-os-vários-mallocs-da-aplicação-em-uma-struct
/*   Updated: 2023/01/14 16:13:06 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	verification(char **args);

/*
*	description: will exit the shell and return a value in the 256 range
*	args: a single number
*	return: on exit returns zero or args % 256 if given 
*			on error it doesn't exit and returns 1
*/

// TO-DO: it should actually print to STDERR
int	ft_exit(t_info *shell_data)
{
	int		i;
	int		exit_code;
	char	**args;

	args = shell_data->cmd;
	i = 1;
	verification(args);
	while (args[i] != NULL)
		i++;
	printf("exit\n");
	if (i > 2)
		return (set_error("exit: too many arguments\n", 1, NULL));
	i = 0;
	g_exit_code = 0;
	if (args[1])
		g_exit_code = (int)ft_atoi(args[1]) % 256;
  destroy_shell(shell_data);	
  exit(g_exit_code);
	return (0);
}

void	exit_error(char **args, int i)
{
	if (args[1][i])
	{
		ft_putstr_fd("exit\nMinishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		set_error(": numeric argument required\n", 2, NULL);
		exit(g_exit_code);
	}
	exit_code = convert_to_range(args[1]);
	destroy_shell(shell_data);
	exit (exit_code);
	return (0);
}

void	verification(char **args)
{
	int	i;

	i = 0;
	if (!args[1])
		return ;
	while (ft_isdigit(args[1][i]) || args[1][0] == '-' || args[1][0] == '+')
	{
		if ((args[1][0] == '-' && args[1][1] == 0)
				|| (args[1][0] == '+' && args[1][1] == 0))
			exit_error(args, i);
		if (!args[1][++i])
			break ;
	}
	exit_error(args, i);
}
