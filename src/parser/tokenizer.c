#include "minishell.h"

// New function to handle concatenated tokens (quoted + unquoted)
int process_concatenated_word(const char *input, int i, t_token **head)
{
    char *word_parts[256]; // Array to hold word parts
    int part_count = 0;
    int expandable = 0; // Will be 1 if any part is expandable
    int start_pos = i;
    
    // Process all consecutive parts that should be concatenated
    while (input[i] && !is_token_delimiter(input[i]) && !is_operator_char(input[i]))
    {
        if (is_quote_char(input[i]))
        {
            // Handle quoted part
            char quote_char = input[i];
            int quote_start = i + 1;
            int quote_end = find_closing_quote(input, i, quote_char);
            
            if (quote_end == -1)
            {
                // Unclosed quote error
                for (int k = 0; k < part_count; k++)
                    free(word_parts[k]);
                return (-1);
            }
            
            // Extract quoted content
            int content_len = quote_end - quote_start;
            word_parts[part_count] = malloc(content_len + 1);
            if (!word_parts[part_count])
            {
                for (int k = 0; k < part_count; k++)
                    free(word_parts[k]);
                return (quote_end + 1);
            }
            
            copy_quoted_text(input, quote_start, quote_end, word_parts[part_count], quote_char);
            
            // Set expandable flag if this is a double-quoted part
            if (quote_char == '"')
                expandable = 1;
                
            part_count++;
            i = quote_end + 1;
        }
        else
        {
            // Handle unquoted part
            int word_start = i;
            while (input[i] && !is_token_delimiter(input[i]) && 
                   !is_operator_char(input[i]) && !is_quote_char(input[i]))
            {
                if (input[i] == '\\' && input[i + 1])
                    i += 2;
                else
                    i++;
            }
            
            int word_len = i - word_start;
            word_parts[part_count] = malloc(word_len + 1);
            if (!word_parts[part_count])
            {
                for (int k = 0; k < part_count; k++)
                    free(word_parts[k]);
                return (i);
            }
            
            copy_word_chars(input, word_start, word_parts[part_count], word_len);
            expandable = 1; // Unquoted parts are expandable
            part_count++;
        }
    }
    
    // If we only processed one part and it started at a quote, 
    // use the original quote handling logic
    if (part_count == 1 && is_quote_char(input[start_pos]))
    {
        // Create the token with the single part
        t_token *new_token = create_token(TOKEN_WORD, word_parts[0], 
                                         (input[start_pos] == '"') ? 1 : 0);
        if (new_token)
            add_token(head, new_token);
        else
            free(word_parts[0]);
        return (i);
    }
    
    // Concatenate all parts into a single token value
    int total_len = 0;
    for (int j = 0; j < part_count; j++)
        total_len += ft_strlen(word_parts[j]);
    
    char *concatenated = malloc(total_len + 1);
    if (!concatenated)
    {
        for (int k = 0; k < part_count; k++)
            free(word_parts[k]);
        return (i);
    }
    
    concatenated[0] = '\0';
    for (int j = 0; j < part_count; j++)
    {
        strcat(concatenated, word_parts[j]);
        free(word_parts[j]);
    }
    
    // Create the concatenated token
    t_token *new_token = create_token(TOKEN_WORD, concatenated, expandable);
    if (new_token)
        add_token(head, new_token);
    else
        free(concatenated);
    
    return (i);
}

t_token *tokenize(const char *input)
{
    t_token *head;
    int i;
    int result;
    char *processed_input;

    processed_input = handle_newlines(input);
    if (!processed_input)
        return (NULL);

    head = NULL;
    i = 0;
    while (processed_input[i])
    {
        i = skip_white_space(processed_input, i);
        if (!processed_input[i])
            break ;
        result = process_token(processed_input, i, &head);
        if (result == -1)
        {
            free(processed_input);
            return (NULL);
        }
        i = result;
    }

    free(processed_input);
    return (head);
}