/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:28:24 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/16 21:25:28 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		g_exit_code = 128 + SIGINT;
	}	
	else if (i != 42)
	{
		write(1, "\n", 1);
		rl_replace_line("", 0);
		rl_redisplay();
		if (WIFSIGNALED(i))
			g_exit_code = 128 + WTERMSIG(i);
	}
}

void	handle_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
