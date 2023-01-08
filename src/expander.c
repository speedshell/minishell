/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpinna-l <mpinna-l@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/07 09:29:03 by mpinna-l          #+#    #+#             */
/*   Updated: 2023/01/08 19:22:06 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

#define ERR_MALLOC 0

char	*expand_variable(char *input, char **env, int *variable_size);
void	add_var_value(t_list **str_nds, char *input, int *i, char **env);
char	*str_nodes_join(t_list *str_nodes);

char	*expand_str(char *input, char **env)
{
	t_list	*str_nodes;
	int		str_size;
	int		i;

	str_size = ft_strlen(input);
	i = 0;
	str_nodes = NULL;
	while (i < str_size)
	{
		if (input[i] && input[i] != '$')
		{
			ft_lstadd_back(&str_nodes, ft_lstnew(ft_substr(input, i, 1)));
			i++;
			continue ;
		}
		add_var_value(&str_nodes, input, &i, env);
	}
	if (!str_nodes)
		return (NULL);
	return (str_nodes_join(str_nodes));
}

void	add_var_value(t_list **str_nodes, char *input, int *i, char **env)
{
	char	*variable;
	int		variable_size;
	int		str_size;

	str_size = ft_strlen(input);
	variable_size = 0;
	variable = NULL;
	if (*i < str_size)
	{
		variable = expand_variable(&input[*i], env, &variable_size);
		ft_lstadd_back(str_nodes, ft_lstnew(variable));
		if (input[*i] == '$' && variable_size == 0)
			ft_lstadd_back(str_nodes, ft_lstnew(ft_substr(input, *i, 1)));
		if (variable_size != 0)
			*i = *i + variable_size + 2;
		else
			*i = *i + 1;
		variable_size = 0;
	}
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

char	*str_nodes_join(t_list *str_nodes)
{
	int		str_size;
	t_list	*node;
	char	*expanded_str;

	node = str_nodes;
	while (node)
	{
		str_size += ft_strlen(((char *) node->content));
		node = node->next;
	}
	expanded_str = ft_calloc(sizeof(char), (str_size + 1));
	while (str_nodes)
	{
		ft_strlcat(expanded_str, (char *) str_nodes->content, str_size);
		node = str_nodes;
		str_nodes = str_nodes->next;
		free(node->content);
		free(node);
	}
	return (expanded_str);
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
