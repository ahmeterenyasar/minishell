#include "minishell.h"

t_token *tokenize(const char *input)
{
    t_token	*head;
    t_token	*current;
    int		i;

    if (!input || !*input)
        return (NULL);
    head = NULL;
    current = NULL;
    i = 0;

    /*
    Whitespace handling:
    Quotes handling:
    Redirection handling:
    Word handling:
        expension in double quotes
        no expension in single quotes
    */

    while (input[i])
    {
        if (is_whitespace(input[i]))
            i++;
        else if (is_quote(input[i]))
            current = handle_quotes(input, &i, current);
        else if (is_redirection(input[i]))
            current = handle_redirection(input, &i, current);
        else
            current = handle_word(input, &i, current);
    }
    return (head);
}