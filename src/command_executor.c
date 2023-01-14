/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:29:48 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/14 18:49:15 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		execute_builtin(t_info *shell_data, int builtin_id);
void	redirect_setup(t_command *expr);
void	fds_close(t_command *expr);

void	command_executor(t_info *shell_data)
{
	int		pid;
	int		builtin_id;
	char	*cmd_path;

	cmd_path = shell_data->cmd[0];
	if (!cmd_path)
		return ;
	builtin_id = is_builtin(cmd_path);
	if (builtin_id != -1)
	{
		execute_builtin(shell_data, builtin_id);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		print_err_msg();
		return ;
	}
	if (pid == 0)
	{
		pipes_setup(shell_data->expr);
		redirect_setup(shell_data->expr);
		if (execve(cmd_path, shell_data->cmd, shell_data->env) == -1)
		{
			print_err_msg();
			pipes_close(shell_data->expr);
			fds_close(shell_data->expr);
			destroy_shell(shell_data);
			exit(0);
		}
	}	
	else
	{
		pipes_close(shell_data->expr);
		fds_close(shell_data->expr);
	}
}

void	fds_close(t_command *expr)
{
	int	*redirect;

	redirect = expr->redirect;
	if (redirect[0] != -1)
		close(redirect[0]);
	if (redirect[1] != -1)
		close (redirect[1]);
}

void	redirect_setup(t_command *expr)
{
	if (expr->redirect[0] != -1)
		dup2(expr->redirect[0], STDIN_FILENO);
	if (expr->redirect[1] != -1)
		dup2(expr->redirect[1], STDOUT_FILENO);
}

/*
 *	description: checks if the cmd is one of the builtins
 *	args: a command name in string format (ex: echo, env, pwd, etc)
 *	return: the const number associated with the builtin, -1 if not a builtin
*/

int	is_builtin(char *cmd_path)
{
	int	cmd_size;

	if (!cmd_path)
		return (-1);
	cmd_size = ft_strlen(cmd_path);
	if (ft_strncmp(cmd_path, "echo", cmd_size) == 0)
		return (ECHO);
	if (ft_strncmp(cmd_path, "exit", cmd_size) == 0)
		return (EXIT);
	if (ft_strncmp(cmd_path, "env", cmd_size) == 0)
		return (ENV);
	if (ft_strncmp(cmd_path, "pwd", cmd_size) == 0)
		return (PWD);
	if (ft_strncmp(cmd_path, "cd", cmd_size) == 0)
		return (CD);
	if (ft_strncmp(cmd_path, "export", cmd_size) == 0)
		return (EXPORT);
	if (ft_strncmp(cmd_path, "unset", cmd_size) == 0)
		return (UNSET);
	return (-1);
}

/*
 * description: executes the builtin function
 * args: args = the function args, builtin_id = the builtin id
 * return: the builtin functions's return code
*/

int	execute_builtin(t_info *shell_data, int builtin_id)
{
	int	op_code;
	int	std_backup[2];

	op_code = 0;
	std_backup[0] = -1;
	std_backup[1] = -1;
	pipes_builtin_setup(shell_data->expr, std_backup);
	redirection_builtin_setup(shell_data->expr, std_backup);
	if (builtin_id == ECHO)
		op_code = ft_echo(shell_data->cmd);
	if (builtin_id == EXIT)
		ft_exit(shell_data);
	if (builtin_id == ENV)
		op_code = ft_env(shell_data->cmd, shell_data->env);
	if (builtin_id == PWD)
		op_code = ft_pwd(shell_data->cmd, shell_data->env);
	if (builtin_id == CD)
		op_code = ft_cd(shell_data->cmd, shell_data->env);
	if (builtin_id == EXPORT)
		op_code = ft_export(shell_data);
	if (builtin_id == UNSET)
		op_code = ft_unset(shell_data);
	pipes_builtin_close(shell_data->expr, std_backup);
	redirection_builtin_close(shell_data->expr, std_backup);
	return (op_code);
}
