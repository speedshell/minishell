/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lfarias- <lfarias-@student.42.rio>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/05 13:34:26 by lfarias-          #+#    #+#             */
/*   Updated: 2023/01/15 15:00:40 by lfarias-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_list		*make_tokens(char *user_input);
t_command	*create_expression(void);
int			count_tokens(t_list *token_list);
void		set_expr_attr(t_token *tkn, t_command *cmd, int *is_pipe_chain);

t_command	*parse_expression(t_list **token_list)
{
	t_token		*tkn;
	t_command	*cmd;
	int			i;
	int			token_qty;
	static int	is_pipe_chain;

	token_qty = count_tokens(*token_list);
	if (token_qty == 0)
		return (NULL);
	cmd = create_expression();
	cmd->tokens = malloc(sizeof(t_token *) * (token_qty + 1));
	i = -1;
	while (++i < token_qty && *token_list)
	{
		tkn = (t_token *)(*token_list)->content;
		cmd->tokens[i] = tkn;
		set_expr_attr(tkn, cmd, &is_pipe_chain);
		*token_list = (*token_list)->next;
	}
	cmd->tokens[i] = NULL;
	if (*token_list == NULL || i > token_qty)
		is_pipe_chain = 0;
	return (cmd);
}

void	set_expr_attr(t_token *tkn, t_command *cmd, int *is_pipe_chain)
{
	if (tkn->type == PIPE)
	{
		cmd->has_pipe = 1;
		*is_pipe_chain = 1;
	}
	if (tkn->type == REDIRECT)
		cmd->has_redirect = 1;
	if (*is_pipe_chain)
		cmd->pipe_chain = 1;
}

t_command	*create_expression(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(*cmd));
	if (!cmd)
		return (NULL);
	cmd->has_pipe = 0;
	cmd->pipe_chain = 0;
	cmd->has_redirect = 0;
	cmd->builtin = 0;
	cmd->redirect[0] = -1;
	cmd->redirect[1] = -1;
	cmd->in_pipe[0] = -1;
	cmd->in_pipe[1] = -1;
	cmd->out_pipe[0] = -1;
	cmd->out_pipe[1] = -1;
	return (cmd);
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
