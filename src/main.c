/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/27 08:46:06 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/13 21:13:44 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <unistd.h>

int		g_exit_code;

void	handle_eof(char *input)
{
	if (!input)
	{
		printf("exit\n");
		exit(0);
	}
	else
		free(input);
}

int	main(int argc, char **argv, char **env)
{
	char		*read_line_buffer;
	t_list		*tokens;
	t_env		env_clone;

	g_exit_code = 0;
	(void)argv;
	read_line_buffer = NULL;
	if (argc != 1)
		return (set_error("Too many args. Usage: ./minishell\n", 1, NULL));
	handle_signals();
	env_clone.env = build_env(env);
	while (42)
	{
		read_line_buffer = readline("Minishell> ");
		if (read_line_buffer && *read_line_buffer)
		{
			add_history(read_line_buffer);
			tokens = make_tokens(read_line_buffer);
			eval_tokens(&tokens, &env_clone);
			ft_lstclear(&tokens, free_token);
		}
		handle_eof(read_line_buffer);
		if (access(".here_doc", F_OK) == 0)
			unlink(".here_doc");
	}
	free2d((void **) env_clone.env);
	rl_clear_history();
	return (0);
}
