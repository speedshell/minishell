#include "../includes/minishell.h"
#include <sys/wait.h>

void	command_executor(char *cmd_path, char **args, char **env)
{
	int	pid;
	int wstatus;

	if (!cmd_path)
		return ;
	pid = fork();
	if (pid == 0)
	{
		if (execve(cmd_path, args, env) == -1)
			printf("Command not found\n");
		exit(0);
	}	
	else
		wait(&wstatus);
}
