/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:29:48 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/12 16:25:57 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		execute_builtin(char **args, t_env *env, int b_id, t_command *expr);
void	redirect_setup(t_command *expr);
void	fds_close(t_command *expr);

void	command_executor(char **cmd_and_args, t_command *expr, t_env *env)
{
	int		pid;
	int		wstatus;
	int		builtin_id;
	char	*cmd_path;

	cmd_path = cmd_and_args[0];
	if (!cmd_path)
		return ;
	builtin_id = is_builtin(cmd_path);
	if (builtin_id != -1)
	{
		execute_builtin(cmd_and_args, env, builtin_id, expr);
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
		pipes_setup(expr);
		redirect_setup(expr);
		if (execve(cmd_path, cmd_and_args, env->env) == -1)
			print_err_msg();
		exit(0);
	}	
	else
	{
		pipes_close(expr);
		wait(&wstatus);
		fds_close(expr);
		expr->return_code = wstatus;
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
	if (ft_strncmp(cmd_path, "echo", 4) == 0)
		return (ECHO);
	if (ft_strncmp(cmd_path, "exit", 4) == 0)
		return (EXIT);
	if (ft_strncmp(cmd_path, "env", 3) == 0)
		return (ENV);
	if (ft_strncmp(cmd_path, "pwd", 3) == 0)
		return (PWD);
	if (ft_strncmp(cmd_path, "cd", 2) == 0)
		return (CD);
	if (ft_strncmp(cmd_path, "export", 6) == 0)
		return (EXPORT);
	if (ft_strncmp(cmd_path, "unset", 5) == 0)
		return (UNSET);
	return (-1);
}

/*
 * description: executes the builtin function
 * args: args = the function args, builtin_id = the builtin id
 * return: the builtin functions's return code
*/

int	execute_builtin(char **args, t_env *env, int builtin_id, t_command *expr)
{
	int	op_code;
	int	std_backup[2];

	op_code = 0;
	std_backup[0] = -1;
	std_backup[1] = -1;
	pipes_builtin_setup(expr, std_backup);
	redirection_builtin_setup(expr, std_backup);
	if (builtin_id == ECHO)
		op_code = ft_echo(args);
	if (builtin_id == EXIT)
		ft_exit(args);
	if (builtin_id == ENV)
		op_code = ft_env(env->env);
	if (builtin_id == PWD)
		op_code = ft_pwd(args, env->env);
	if (builtin_id == CD)
		op_code = ft_cd(args, env->env);
	if (builtin_id == EXPORT)
		op_code = ft_export(args, env);
	if (builtin_id == UNSET)
		op_code = ft_unset(args, env);
	pipes_builtin_close(expr, std_backup);
	redirection_builtin_close(expr, std_backup);
	return (op_code);
}

