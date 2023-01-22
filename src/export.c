/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:28:50 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/22 16:46:11 by mpinna-l         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	export_err_msg(char *str);

void	replace(int i, t_info *shell_data, char *variable, char *value)
{
	char	**new_env;

	new_env = malloc(sizeof(char *) * (i + 2));
	i = 0;
	while (shell_data->env[i])
	{
		new_env[i] = ft_strdup(shell_data->env[i]);
		free(shell_data->env[i++]);
	}
	free(shell_data->env[i]);
	free(shell_data->env);
	new_env[i++] = ft_strjoin(variable, value);
	new_env[i] = NULL;
	shell_data->env = new_env;
	return ;
}

void	search_and_replace(char *variable, t_info *shell_data, char *value)
{
	int		i;
	int		len;
	char	*temp;

	i = -1;
	len = ft_strlen(variable);
	while (shell_data->env[++i])
	{
		if (!ft_strncmp(variable, shell_data->env[i], len))
		{
			temp = shell_data->env[i];
			shell_data->env[i] = ft_strjoin(variable, value);
			free(temp);
			return ;
		}
	}
	replace(i, shell_data, variable, value);
}

void	extract(t_info *shell_data, char *args, int flag)
{
	int		i;
	char	*variable;
	char	*value;

	i = 0;
	while (args[i] != '=')
		i++;
	variable = ft_substr(args, 0, i + 1);
	value = ft_strdup(ft_strchr(args, '=') + 1);
	if (flag == 0)
		search_and_replace(variable, shell_data, value);
	if (flag == 1)
	{
		export_err_msg(args);
	}
	free(variable);
	free(value);
}

void	export_err_msg(char *str)
{
	ft_putstr_fd("Minishell: export: `", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	g_exit_code = 1;
}

int	ft_export(t_info *shell_data)
{
	int		j;
	char	**args;

	j = 0;
	args = shell_data->cmd;
	if (!args[1])
		return (ft_env(shell_data->cmd, shell_data->env));
	while (args[++j])
	{
		if (valid_variable(args[j]) && (ft_strchr(args[j], '=')))
			extract(shell_data, args[j], 0);
		else
		{
			if (ft_strchr(args[j], '='))
				extract(shell_data, args[j], 1);
			else
				export_err_msg(args[j]);
		}
	}
	return (g_exit_code);
}
