/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/06 15:21:10 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/21 18:48:45 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	fallback_pwd(char **env);

/*
*	description: will print the current directory
*	args: args - to see if it was invoked with invalid args
*		  env - the env table
*	return: on success 0, on failure 1 (no permission, corrupt dirs, etc)
*/

// TO-DO: print error on STDERROR
int	ft_pwd(t_info *shell_data)
{
	char	*curr_dir;

	curr_dir = NULL;
	curr_dir = getcwd(NULL, 0);
	if (curr_dir != NULL)
	{
		printf("%s\n", curr_dir);
		free(shell_data->pwd);
		shell_data->pwd = curr_dir;
		return (0);
	}
	else
		printf("%s\n", shell_data->pwd);
	return (0);
}

/*
*	description: if getcwd fails will use the PWD env var as a fallback
*	args: env - the enviroment variables table
*	output: if PWD exists will use its value to print it, if not print error
*	return: on success 0, on error 1
*/

int	fallback_pwd(char **env)
{
	int	i;

	i = 0;
	while (env[i] && ft_strncmp("PWD=", env[i], 4) != 0)
		i++;
	if (env[i] != NULL)
	{
		printf("%s\n", &env[i][4]);
		return (0);
	}
	else
	{
		print_err_msg();
		return (1);
	}	
}
