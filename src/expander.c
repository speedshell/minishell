/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:29:03 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/23 16:30:51 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define ERR_MALLOC 0

char	*expand_variable(char *input, char **env, int *variable_size);
int		add_var_value(t_list **str_nds, char *input, int *expand, char **env);
int		is_expandable(char *input, int i, int *expand);
char	*get_exit_str(char *input, int *variable_size);
int		valid_variable(char *c);

char	*expand_str(char *input, char **env)
{
	t_list	*str_nodes;
	int		str_size;
	int		i;
	int		expand;

	expand = 2;
	str_nodes = NULL;
	i = 0;
	if (!input)
		return (NULL);
	str_size = ft_strlen(input);
	while (i < str_size)
	{
		if (input[i] && input[i] != '$')
		{
			is_expandable(input, i, &expand);
			ft_lstadd_back(&str_nodes, ft_lstnew(ft_substr(input, i, 1)));
			i++;
			continue ;
		}
		i += add_var_value(&str_nodes, &input[i], &expand, env);
	}
	return (str_nodes_join(str_nodes));
}

int	add_var_value(t_list **str_nds, char *input, int *expand, char **env)
{
	char	*variable;
	int		vsize;
	int		str_size;
	int		i;

	str_size = ft_strlen(input);
	i = 0;
	if (i < str_size)
	{
		variable = expand_variable(&input[i], env, &vsize);
		if (is_expandable(input, i, expand))
			ft_lstadd_back(str_nds, ft_lstnew(variable));
		else
		{
			free(variable);
			ft_lstadd_back(str_nds, ft_lstnew(ft_substr(input, i, vsize + 2)));
		}
		if (input[i] == '$' && vsize == 0)
			ft_lstadd_back(str_nds, ft_lstnew(ft_substr(input, i, 1)));
		if (vsize != 0)
			i = i + vsize + 2;
		else
			i = i + 1;
	}
	return (i);
}

char	*expand_variable(char *input, char **env, int *variable_size)
{
	int		i;
	char	*variable;

	*variable_size = 0;
	i = -1;
	if (!input)
		return (NULL);
	if (*input == '$')
	{
		if (*(input + 1) == '?')
			return (expand_exit_variable(variable_size));
		*variable_size = valid_variable(++input);
		if (*variable_size != 0 && *input)
		{
			while (env[++i])
			{
				if (!ft_strncmp(input, env[i], ft_strchr(env[i], '=') - env[i]))
				{
					variable = ft_strdup(ft_strchr(env[i], '=') + 1);
					return (variable);
				}
			}
		}	
	}
	return (NULL);
}

int	is_expandable(char *input, int i, int *expand)
{
	if (input[i] == '\'' && *expand == 2)
	{
		*expand = 0;
	}
	else if (input[i] == '\'' && *expand == 0)
	{
		*expand = 2;
	}
	else if (input[i] == '"' && *expand == 2)
	{
		*expand = 4;
	}
	else if (input[i] == '"' && *expand == 4)
	{
		*expand = 2;
	}
	return (*expand);
}

int	valid_variable(char *c)
{
	int	i;

	i = 0;
	while (c[i] && c[i] != '$')
	{
		if (c[i + 1] && c[i + 1] == '$')
			return (i);
		if (c[i + 1] && c[i + 1] == ' ')
			return (i);
		if ((c[i + 1] && c[i + 1] == '"') || (c[i + 1] && c[i + 1] == '\''))
			return (i);
		if (c[i] == '=' && i == 0)
			return (0);
		else if (c[i + 1] == '=' && i != 0)
			break ;
		else if ((!ft_isalpha(c[i]) && !(c[i] == '_')) && i == 0)
			return (0);
		else if ((!ft_isalpha(c[i]) && !ft_isdigit(c[i]) && !(c[i] == '_')))
			return (0);
		i++;
	}
	return (i);
}
