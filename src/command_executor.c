#include "../includes/minishell.h"

void	command_executor(char *cmd_path, char **args, char **env)
{
	int	pid;
	int	wstatus;

	if (!cmd_path)
		return ;
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
