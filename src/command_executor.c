/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_executor.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:29:48 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/21 18:48:26 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		exec_forked_cmd(t_info *shell_data);
int		exec_builtin(t_info *shell_data, int builtin_id);
int		exec_unforked_builtin(t_info *shell_data, int builtin_id);

void	command_executor(t_info *shell_data)
{
	int			pid;

	if (!shell_data->cmd || !shell_data->cmd[0])
		return ;
	if (shell_data->expr->pipe_chain == 0 && shell_data->expr->builtin != -1)
	{
		exec_unforked_builtin(shell_data, shell_data->expr->builtin);
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		print_err_msg();
		return ;
	}
	else if (pid == 0)
		exit(exec_forked_cmd(shell_data));
	else
	{
		lst_pid_add_back(&shell_data->child_pids, pid);
		pipes_close(shell_data->expr);
		redirect_close(shell_data->expr);
	}
}

int	exec_forked_cmd(t_info *shell_data)
{
	char		*cmd_path;
	t_command	*expr;
	int			op_code;

	handle_child_signals();
	op_code = 0;
	cmd_path = shell_data->cmd[0];
	expr = shell_data->expr;
	pipes_setup(expr);
	redirect_setup(expr);
	if (expr->pipe_chain == 1 && expr->builtin != -1)
		op_code = exec_builtin(shell_data, expr->builtin);
	else if (execve(cmd_path, shell_data->cmd, shell_data->env) == -1)
	{
		if (errno == ENOENT)
			op_code = 127;
		if (errno == EACCES)
			op_code = 126;
		print_err_str(cmd_path, op_code);
	}
	pipes_close(expr);
	redirect_close(expr);
	destroy_shell(shell_data);
	return (op_code);
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
	if (ft_strncmp(cmd_path, "echo", 4) == 0 && cmd_size == 4)
		return (ECHO);
	if (ft_strncmp(cmd_path, "exit", 4) == 0 && cmd_size == 4)
		return (EXIT);
	if (ft_strncmp(cmd_path, "env", 3) == 0 && cmd_size == 3)
		return (ENV);
	if (ft_strncmp(cmd_path, "pwd", 3) == 0 && cmd_size == 3)
		return (PWD);
	if (ft_strncmp(cmd_path, "cd", 2) == 0 && cmd_size == 2)
		return (CD);
	if (ft_strncmp(cmd_path, "export", 6) == 0 && cmd_size == 6)
		return (EXPORT);
	if (ft_strncmp(cmd_path, "unset", 5) == 0 && cmd_size == 5)
		return (UNSET);
	return (-1);
}

/*
 * description: executes the builtin function
 * args: args = the function args, builtin_id = the builtin id
 * return: the builtin functions's return code
*/

int	exec_unforked_builtin(t_info *shell_data, int builtin_id)
{
	int	op_code;
	int	std_backup[2];

	std_backup[0] = -1;
	std_backup[1] = -1;
	pipes_builtin_setup(shell_data->expr, std_backup);
	redirection_builtin_setup(shell_data->expr, std_backup);
	op_code = exec_builtin(shell_data, builtin_id);
	pipes_builtin_close(shell_data->expr, std_backup);
	redirection_builtin_close(shell_data->expr, std_backup);
	return (op_code);
}

int	exec_builtin(t_info *shell_data, int builtin_id)
{
	int	op_code;

	op_code = 0;
	if (builtin_id == ECHO)
		op_code = ft_echo(shell_data->cmd);
	if (builtin_id == EXIT)
		ft_exit(shell_data);
	if (builtin_id == ENV)
		op_code = ft_env(shell_data->cmd, shell_data->env);
	if (builtin_id == PWD)
		op_code = ft_pwd(shell_data);
	if (builtin_id == CD)
		op_code = ft_cd(shell_data);
	if (builtin_id == EXPORT)
		op_code = ft_export(shell_data);
	if (builtin_id == UNSET)
		op_code = ft_unset(shell_data);
	return (op_code);
}
