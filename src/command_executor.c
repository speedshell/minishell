#include "../includes/minishell.h"

int	execute_builtin(char **args, t_env *env, int builtin_id);

void	command_executor(char *cmd_path, char **args, t_env *env)
{
	int	pid;
	int	wstatus;
	int	builtin_id;

	if (!cmd_path)
		return ;
	builtin_id = is_builtin(cmd_path);
	if (builtin_id != -1)
	{
		execute_builtin(args, env, builtin_id);
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
		if (execve(cmd_path, args, env->env) == -1)
			print_err_msg();
		exit(0);
	}	
	else
		wait(&wstatus);
}

/*
 *	description: checks if the cmd is one of the builtins
 *	args: a command name in string format (ex: echo, env, pwd, etc)
 *	return: the const number associated with the builtin, -1 if not a builtin
*/

int	is_builtin(char *cmd_path)
{
	if (strncmp(cmd_path, "echo", 4) == 0)
		return (ECHO);
	if (strncmp(cmd_path, "exit", 4) == 0)
		return (EXIT);
	if (strncmp(cmd_path, "env", 3) == 0)
		return (ENV);
	if (strncmp(cmd_path, "pwd", 3) == 0)
		return (PWD);
	if (strncmp(cmd_path, "export", 6) == 0)
		return (EXPORT);
	return (-1);
}

/*
 * description: executes the builtin function
 * args: args = the function args, builtin_id = the builtin id
 * return: the builtin functions's return code
*/

int	execute_builtin(char **args, t_env *env, int builtin_id)
{
	int	op_code;

	op_code = 0;
	if (builtin_id == ECHO)
		op_code = ft_echo(args);
	if (builtin_id == EXIT)
		ft_exit(args);
	if (builtin_id == ENV)
		op_code = ft_env(env->env);
	if (builtin_id == PWD)
		op_code = ft_pwd(args, env->env);
	if (builtin_id == EXPORT)
		op_code = ft_export(args, env);
	return (op_code);
}
