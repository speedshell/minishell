/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/30 02:13:07 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/22 16:20:28 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/stat.h>       
#include <sys/types.h>
#include <dirent.h>

int		go_home(t_info *shell_data);
int		find_env(char *env_name, char **env);
void	update_env_vars(t_info *shell_data);
int		no_pwd(char **env, int pwd_i, int old_pwd_i, t_info *shell_data);
void	cd_sandbox(t_info *shell_data, int *w_status);

/*
*	description: changes directory
*/

int	ft_cd(t_info *shell_data)
{
	int		i;
	int		w_status;

	i = 0;
	w_status = 0;
	while (shell_data->cmd[i])
		i++;
	if (i > 2)
	{
		set_error("Minishell: cd: too many arguments\n", 1, NULL);
		return (g_exit_code);
	}
	if (i > 1 && shell_data->cmd[1][0] == '\0')
		shell_data->cmd[1][0] = '.';
	if (i == 1)
		return (go_home(shell_data));
	cd_sandbox(shell_data, &w_status);
	if (w_status == 0)
	{
		chdir(shell_data->cmd[1]);
		update_env_vars(shell_data);
	}
	else if (w_status != 0)
		set_cd_error("Minishell: cd: ", w_status, shell_data->cmd);
	return (g_exit_code);
}

void	cd_sandbox(t_info *shell_data, int *w_status)
{
	int		pid;
	char	*folder_path;

	folder_path = NULL;
	pid = fork();
	if (pid == 0)
	{
		if (chdir(shell_data->cmd[1]) == -1)
			*w_status = errno;
		folder_path = getcwd(NULL, 0);
		destroy_shell(shell_data);
		if (!folder_path)
			exit(errno);
		else
		{
			free(folder_path);
			exit(*w_status);
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

void	update_env_vars(t_info *shell_data)
{
	int		pwd_i;
	int		old_pwd_i;
	char	*aux1;

	pwd_i = find_env("PWD=", shell_data->env);
	old_pwd_i = find_env("OLDPWD=", shell_data->env);
	if (no_pwd(shell_data->env, pwd_i, old_pwd_i, shell_data))
		return ;
	if (old_pwd_i != -1)
	{
		aux1 = ft_strjoin("OLDPWD=", shell_data->pwd);
		free(shell_data->env[old_pwd_i]);
		shell_data->env[old_pwd_i] = aux1;
	}
	free(shell_data->pwd);
	shell_data->pwd = getcwd(NULL, 0);
	if (pwd_i != -1)
	{
		aux1 = ft_strjoin("PWD=", shell_data->pwd);
		free(shell_data->env[pwd_i]);
		shell_data->env[pwd_i] = aux1;
	}
}

/* 
*	description: if there's no PWD but OLDPWD exists then OLDPWD will be empty
*/

int	no_pwd(char **env, int pwd_i, int old_pwd_i, t_info *shell_data)
{
	char	*aux;

	g_exit_code = 0;
	if (pwd_i == -1 && old_pwd_i >= 0)
	{
		aux = ft_strdup("OLDPWD=");
		free(env[old_pwd_i]);
		env[old_pwd_i] = aux;
		free(shell_data->pwd);
		shell_data->pwd = getcwd(NULL, 0);
		return (1);
	}
	return (0);
}

/*
*	description: cd invoked without arguments should go to HOME directory
*	notes: will fail if HOME is no present on env
*	args: the env vars table
*/

int	go_home(t_info *shell_data)
{
	char	*home_path;
	int		i;
	int		op_code;

	i = find_env("HOME=", shell_data->env);
	if (i == -1)
	{
		set_error("Minishell: cd: HOME not set\n", 1, NULL);
		return (g_exit_code);
	}
	home_path = &shell_data->env[i][5];
	op_code = chdir(home_path);
	if (op_code != 0)
		print_err_msg();
	else
		update_env_vars(shell_data);
	return (op_code);
}
