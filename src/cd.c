/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 02:13:07 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/19 19:34:42 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/stat.h>       
#include <sys/types.h>
#include <dirent.h>

int		go_home(char **env);
int		find_env(char *env_name, char **env);
void	update_env_vars(char **env);
int		no_pwd(char **env, int pwd_i, int old_pwd_i);

/*
*	description: changes directory
*/

int	ft_cd(t_info *shell_data)
{
	int		i;
	int		op_code;
	int		w_status;

	i = 0;
	op_code = 0;
	while (args[i])
		i++;
	if (i > 2)
	{
		set_error("Minishell: cd: too many arguments\n", 1, NULL);
		return (g_exit_code);
	}
	if (i == 1)
	{
		op_code = go_home(env);
		return (op_code);
	}
	cd_sandbox(shell_data);
	if (w_status == 0)
	{
		chdir(args[1]);
	}
	if (w_status != 0)
		set_cd_error("Minishell: cd: ", w_status, args);
	else
	{
		update_env_vars(env);
		g_exit_code = op_code;
	}
	return (g_exit_code);
}

void	cd_sandbox(t_info *shell_data, int *w_status)
{
	int		pid;
	char	*folder_path;

	*w_status = 0;
	folder_path = NULL;
	pid = fork();
	if (pid == 0)
	{
		chdir(shell_data->cmd[1]);
		folder_path = getcwd(NULL, 0);
		destroy_shell(shell_data);
		if (!folder_path)
			exit(errno);
		else
		{
			free(folder_path);
			exit(0);
		}
	}
	else
	{
		wait(w_status);
		if (WIFEXITED(*w_status))
			*w_status = WEXITSTATUS(*w_status);
	}
}
/*
*	description: Will update both PWD and OLDPWD if cd is successeful
*	notes: PWD and OLDPWD will only be updated if they exist on env
*	args: the env vars table
*/

void	update_env_vars(char **env)
{
	int		pwd_i;
	int		old_pwd_i;
	char	*aux;
	char	*aux1;

	pwd_i = find_env("PWD=", env);
	old_pwd_i = find_env("OLDPWD=", env);
	if (no_pwd(env, pwd_i, old_pwd_i))
		return ;
	if (old_pwd_i != -1)
	{
		aux = ft_strdup(&env[pwd_i][4]);
		aux1 = ft_strjoin("OLDPWD=", aux);
		free(aux);
		free(env[old_pwd_i]);
		env[old_pwd_i] = aux1;
	}
	if (pwd_i != -1)
	{
		aux = getcwd(NULL, 0);
		aux1 = ft_strjoin("PWD=", aux);
		free(aux);
		free(env[pwd_i]);
		env[pwd_i] = aux1;
	}
}

/* 
*	description: if there's no PWD but OLDPWD exists then OLDPWD will be empty
*/

int	no_pwd(char **env, int pwd_i, int old_pwd_i)
{
	char	*aux;

	if (pwd_i == -1 && old_pwd_i != -1)
	{
		aux = ft_strdup("OLDPWD=");
		free(env[old_pwd_i]);
		env[old_pwd_i] = aux;
		return (1);
	}
	return (0);
}

/*
*	description: cd invoked without arguments should go to HOME directory
*	notes: will fail if HOME is no present on env
*	args: the env vars table
*/

// TO-DO: print error message
int	go_home(char **env)
{
	char	*home_path;
	int		i;
	int		op_code;

	i = find_env("HOME=", env);
	if (i == -1)
	{
		set_error("Minishell: cd: HOME not set\n", 1, NULL);
		return (g_exit_code);
	}
	home_path = &env[i][5];
	op_code = chdir(home_path);
	if (op_code != 0)
		print_err_msg();
	else
		update_env_vars(env);
	return (op_code);
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
