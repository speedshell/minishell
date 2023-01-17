/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:28:24 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/17 19:40:06 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <sys/ioctl.h>

void	sigint_handler(int signo)
{
	int	i;

	i = 42;
	wait(&i);
	if (signo == SIGINT && (i == 42))
	{
		if (RL_ISSTATE(RL_STATE_READCMD))
			ioctl(STDIN_FILENO, TIOCSTI, "\n");
		else
			write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		g_exit_code = 128 + SIGINT;
	}	
	else if (i != 42)
	{
		if (RL_ISSTATE(RL_STATE_READCMD))
			ioctl(STDIN_FILENO, TIOCSTI, "\n");
		else
			write(1, "\n", 1);
		rl_replace_line("", 0);
		if (WIFSIGNALED(i))
			g_exit_code = 128 + WTERMSIG(i);
	}
}

void	handle_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
