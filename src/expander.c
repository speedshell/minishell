#include "../includes/minishell.h"

#define ERR_MALLOC 0

int is_expandable(char c)
{
    if (ft_isalpha(c) || c == '_')
        return (1);
    return (0);
}

char *expanded_str(char *input)
{
    char    *str;
    int     i;
//  char    *variable;
    int     j;

    str = malloc(sizeof(char) * ft_strlen(input) + 1);
    if (!str)
        return (ERR_MALLOC);
    j = 0;
    while (*input)
    {
        if (*input == '$')
        {
            i = 1;
            while (is_expandable(input[i]))
                i++;
    //      variable = ft_substr(input, 0, i);
            input += i;
		    }
        else
            str[j++] = *input++;
		}
    str[j] = 0;
    return (str);
}


int main (int argc, char **argv)
{
    char *teste;

    if (argc == 2 )
    {
        teste = expanded_str(argv[1]);
        printf("%s\n", teste);
    }
    return (0);
}
