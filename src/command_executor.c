#include "../includes/minishell.h"

int	execute_builtin(char **args, char **env, int builtin_id);

void	command_executor(char *cmd_path, char **args, char **env)
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
		if (execve(cmd_path, args, env) == -1)
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
	return (-1);
}

/*
 * description: executes the builtin function
 * args: args = the function args, builtin_id = the builtin id
 * return: the builtin functions's return code
*/

int	execute_builtin(char **args, char **env, int builtin_id)
{
	int	op_code;

	op_code = 0;
	if (builtin_id == ECHO)
		op_code = ft_echo(args)
	if (builtin_id == EXIT)
		ft_exit(args);
	if (builtin_id == ENV)
		op_code = ft_env(env);
	return (op_code);
}
