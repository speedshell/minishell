
#include "../includes/minishell.h"
#include "errno.h"

/* 
*	description: prints a formatted error message to STDERROR
*	return value: the errno associated with that error message
*/

int	print_err_msg(void)
{
	char	*error_string;

	error_string = ft_strjoin("minishell: ", strerror(errno));
	if (!error_string)
	{
		ft_putendl_fd(strerror(errno), 2);
		return (errno);
	}
	ft_putendl_fd(error_string, 2);
	free(error_string);
	return(errno);
}
