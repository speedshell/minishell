/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:34:26 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/11 20:16:18 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list		*make_tokens(char *user_input);
t_token		*get_next_token(t_list *token_list);
t_command	*create_expression(void);
int			count_tokens(t_list *token_list);

t_command	*parse_expression(t_list **token_list)
{
	t_list		*node;
	t_token		*tkn;
	t_command	*cmd;
	int			i;
	int			token_qty;

	token_qty = count_tokens(*token_list);
	if (token_qty == 0)
		return (NULL);
	cmd = create_expression();
	cmd->tokens = malloc(sizeof(t_token *) * (token_qty + 1));
	i = 0;
	while (i < token_qty && token_list)
	{
		tkn = (t_token *)(*token_list)->content;
		cmd->tokens[i] = tkn;
		if (tkn->type == PIPE)
			cmd->has_pipe = 1;
		node = *token_list;
		*token_list = (*token_list)->next;
		free(node);
		i++;
	}
	cmd->tokens[i] = NULL;
	return (cmd);
}

int	check_syntax(t_list *token_list)
{
	t_token	*tkn;
	t_token	*prev_tkn;
	t_token	*next_tkn;

	prev_tkn = NULL;
	next_tkn = NULL;
	while (token_list)
	{
		tkn = (t_token *) token_list->content;
		next_tkn = get_next_token(token_list);
		if (pipe_rules(prev_tkn, tkn, next_tkn) == -1)
			return (0);
		if (redirect_rules(tkn, next_tkn) == -1)
			return (0);
		prev_tkn = tkn;
		token_list = token_list->next;
	}
	return (1);
}

t_command	*create_expression(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->has_pipe = 0;
	cmd->has_redirect = 0;
	cmd->redirect[0] = -1;
	cmd->redirect[1] = -1;
	cmd->in_pipe[0] = -1;
	cmd->in_pipe[1] = -1;
	cmd->out_pipe[0] = -1;
	cmd->out_pipe[1] = -1;
	return (cmd);
}

t_token	*get_next_token(t_list *token_list)
{
	t_token	*tkn;

	tkn = NULL;
	if (token_list->next != NULL)
	{
		tkn = (t_token *) token_list->next->content;
	}
	return (tkn);
}

int	count_tokens(t_list *token_list)
{
	t_list	*node;
	t_token	*tkn;
	int		expression_size;

	node = token_list;
	expression_size = 0;
	while (node)
	{
		tkn = (t_token *) node->content;
		if (tkn->type == PIPE)
			return (++expression_size);
		expression_size++;
		node = node->next;
	}
	return (expression_size);
}

/*void	print_cmd(t_command *cmd)
{
	t_token	*token;
	int		i;

	i = 0;
	while (cmd->tokens[i])
	{
		token = cmd->tokens[i];
		printf("Token type %d\n", token->type);
		printf("Token value %s\n", token->value);
		i++;
	}
}

int	main(void)
{
	char		*input;
	t_list		*tokens;
	t_command	*cmd;

	input = "ls -l | cat -e";
	tokens = make_tokens(input);
	cmd = parse_expression(&tokens);
	print_cmd(cmd);
	cmd = parse_expression(&tokens);
	print_cmd(cmd);
}*/

/*void	print_syntax_ok(char *cmd, int expected)
{
	t_list *token_list;
	int		syntax_ok;

	token_list = make_tokens(cmd);
	syntax_ok = check_syntax(token_list);
	printf("cmd: %s\n", cmd);
	printf("expected: %d, got: %d\n", expected, syntax_ok);
	printf("result: ");
	if (syntax_ok == expected)
	{
		printf("\033[0;32m");
		printf("OK\n");
		printf("\033[0m"); 
	}
	else
	{
		printf("\033[0;31m");
		printf("KO\n");
		printf("\033[0m"); 
	}
	printf("+++\n");	
}

#define FAIL 0 
#define OK 1 

int main(void)
{
	char *cmd;

	cmd = "ls -l";
	print_syntax_ok(cmd, OK);

	cmd = "ls |||";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls |cat -e|";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls |cat -e| echo";
	print_syntax_ok(cmd, OK);

	printf("\n======= '<' SYNTAX CHECK =======\n");
	cmd = "<";
	print_syntax_ok(cmd, FAIL);

	cmd = "< ls";
	print_syntax_ok(cmd, OK);

	cmd = "<ls";
	print_syntax_ok(cmd, OK);

	cmd = "cat -e < infile";
	print_syntax_ok(cmd, OK);

	cmd = "cat -e<infile";
	print_syntax_ok(cmd, OK);

	cmd = "< infile cat -e";
	print_syntax_ok(cmd, OK);

	cmd = "cat -e <";
	print_syntax_ok(cmd, FAIL);

	cmd = "cat -e < <";
	print_syntax_ok(cmd, FAIL);

	cmd = "cat -e < < <";
	print_syntax_ok(cmd, FAIL);

	cmd = "< < cat -e";
	print_syntax_ok(cmd, FAIL);

	printf("\n======= '>' SYNTAX CHECK =======\n");
	cmd = ">";
	print_syntax_ok(cmd, FAIL);

	cmd = "> ls";
	print_syntax_ok(cmd, OK);

	cmd = "> > ls";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls >";
	print_syntax_ok(cmd, FAIL);

	cmd = "> ls >";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls > outfile";
	print_syntax_ok(cmd, OK);

	cmd = "ls > outfile > outfile2";
	print_syntax_ok(cmd, OK);

	cmd = "ls > outfile > outfile2 > outfile3";
	print_syntax_ok(cmd, OK);

	cmd = "ls>outfile>outfile2>outfile3";
	print_syntax_ok(cmd, OK);

	cmd = "ls>outfile>outfile2>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls> >";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls>cat";
	print_syntax_ok(cmd, OK);

	printf("\n======= '>>' SYNTAX CHECK =======\n");
	cmd = ">>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls>>cat";
	print_syntax_ok(cmd, OK);

	cmd = "ls >> cat";
	print_syntax_ok(cmd, OK);

	cmd = "ls >>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls >>>>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls >> cat >>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls>>cat>>";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls >> cat >> echo";
	print_syntax_ok(cmd, OK);

	cmd = "ls>>cat>>echo";
	print_syntax_ok(cmd, OK);

	cmd = ">> ls";
	print_syntax_ok(cmd, OK);
	cmd = ">>ls";
	print_syntax_ok(cmd, OK);

	printf("\n======= '<<' SYNTAX CHECK =======\n");
	cmd = "cat <<";
	print_syntax_ok(cmd, FAIL);

	cmd = "cat << -";
	print_syntax_ok(cmd, OK);

	cmd = "<< cat -e";
	print_syntax_ok(cmd, OK);

	cmd = "<<";
	print_syntax_ok(cmd, FAIL);

	printf("\n======= MISC SYNTAX CHECK =======\n");
	cmd = "ls | cat -e > outfile";
	print_syntax_ok(cmd, OK);

	cmd = "cat | cat -e > outfile | cat -e";
	print_syntax_ok(cmd, OK);

	cmd = "cat -e << - | rev | >";
	print_syntax_ok(cmd, FAIL);

	cmd = "cat -e << - | rev | > infile";
	print_syntax_ok(cmd, OK);

	cmd = "cat << >give";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls <<>> rev";
	print_syntax_ok(cmd, FAIL);

	cmd = "ls | rev | < infile";
	print_syntax_ok(cmd, OK);
}*/
