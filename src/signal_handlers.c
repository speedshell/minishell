#include "../includes/minishell.h"

void	sigint_handler(int signo)
{
	int	i;

	i = 42;
	wait(&i);
	if (signo == SIGINT && (i == 42))
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}	
	else if (signo == SIGQUIT)
		rl_redisplay();
	else if (i != 42)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, sigint_handler);
}
