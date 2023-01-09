/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:29:03 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/08 21:13:48 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define ERR_MALLOC 0

char	*expand_variable(char *input, char **env, int *variable_size);
int		add_var_value(t_list **str_nds, char *input, int *expand, char **env);
char	*str_nodes_join(t_list *str_nodes);
int		is_expandable(char *input, int i, int *expand);

char	*expand_str(char *input, char **env)
{
	t_list	*str_nodes;
	int		str_size;
	int		i;
	int		expand;

	expand = 2;
	str_nodes = NULL;
	i = 0;
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
	vsize = 0;
	variable = NULL;
	i = 0;
	if (i < str_size)
	{
		variable = expand_variable(&input[i], env, &vsize);
		if (is_expandable(input, i, expand))
			ft_lstadd_back(str_nds, ft_lstnew(variable));
		else
			ft_lstadd_back(str_nds, ft_lstnew(ft_substr(input, i, vsize + 2)));
		if (input[i] == '$' && vsize == 0)
			ft_lstadd_back(str_nds, ft_lstnew(ft_substr(input, i, 1)));
		if (vsize != 0)
			i = i + vsize + 2;
		else
			i = i + 1;
		vsize = 0;
	}
	return (i);
}

char	*expand_variable(char *input, char **env, int *variable_size)
{
	int		i;
	char	*variable;

	i = -1;
	if (!input)
		return (NULL);
	if (*input == '$')
	{
		input++;
		*variable_size = valid_variable(input);
		if (*variable_size != 0 && *input)
		{
			while (env[++i])
			{
				if (!ft_strncmp(input, env[i], *variable_size))
				{
					variable = ft_strdup(ft_strchr(env[i], '=') + 1);
					return (variable);
				}
			}
		}	
	}
	return (ft_strdup(""));
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
		else if (c[i] == '=' && i != 0)
			break ;
		else if ((!ft_isalpha(c[i]) && !(c[i] == '_')) && i == 0)
			return (0);
		else if ((!ft_isalpha(c[i]) && !ft_isdigit(c[i]) && !(c[i] == '_')))
			return (0);
		i++;
	}
	return (i);
}

/*int main (int argc, char **argv, char **env)
{
	char *var = "$PWD";
	char *var2 = "$PDW";
	char *var3 = "$#sp";
	char *var4 = "$123";
	char *var5 = "$HOME";
	char *var6 = "$HOME$";
	char *var7 = "$HOME abc $ABC $abc";
	char *var8 = "$HOME abc $ABC $abc bca";
	char *var9 = "$HOME$HOME";
	char *var10 = "Hello $HOME";

	printf("%s expanded -> %s\n", var, expand_str(var, env));
	printf("%s expanded -> %s\n", var2, expand_str(var2, env));
	printf("%s expanded -> %s\n", var3, expand_str(var3, env));
	printf("%s expanded -> %s\n", var4, expand_str(var4, env));
	printf("%s expanded -> %s\n", var5, expand_str(var5, env));
	printf("%s expanded -> %s\n", var6, expand_str(var6, env));
	printf("%s expanded -> %s\n", var7, expand_str(var7, env));
	printf("%s expanded -> %s\n", var8, expand_str(var8, env));
	printf("%s expanded -> %s\n", var9, expand_str(var9, env));
	printf("%s expanded -> %s\n", var10, expand_str(var10, env));
}*/
