#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

#include "simpleshell.h"

char **check_history(char ***history, int *history_len, int *history_index, char **tokens, char *user_in){
    // Handle requests to read from history.
    // check that the first token starts with '!' and there are no other tokens
    if (*tokens[0] == '!' && !*(tokens + 1))
    {
        // if the history is empty then show an appropriate error message and free the tokens.
        if (*history_len < 1)
        {
            empty_history_error();
            return NULL;
        }

        // get the second character of the first token if it is anothe '!' and there are no more
        // characters execute the most recent command in history
        char *command = *tokens + 1;
        if (*command == '!')
        {
            command++;

            // if there are more characters throw an appropriate error
            if (*command != '\0')
            {
                to_many_args_err();
                return NULL;
            }
            else
            {
                tokens = get_tokens(strdup((*history)[(*history_index - 1 + HISTORY_SIZE) % HISTORY_SIZE]));
            }
        }
        else // if the next character ISN'T '!' decide how to handle it checking for digits or a '-'
        {
            // if the next character is '-' set the latest flag to true and move on
            int latest = 0;
            if (*command == '-')
            {
                command++; 
                latest = 1;           
            }

            // parse the remender of the token into a number value from 1 to 20 to be used to find the
            // correct command in history
            int history_value = parseHistoryPosition(command);
            if(history_value == -1) return 0;
            
            // of the latest flag is true get the history starting from the most recent else
            // get it starting from the oldest
            if(latest){
                tokens = get_tokens(strdup((*history)[(*history_index - history_value + *history_len) % *history_len]));
            }
            else{
                tokens = get_tokens(strdup((*history)[((*history_index - 1) +  history_value) % *history_len]));
            }
        }
    }
    // if there is to many parameters then show an appropriate message
    else if (**tokens == '!' && *(tokens + 1))
    {
        to_many_args_err();
        return NULL;
    }
    // if there is no history call then save the entered command into history.
    else
    {
        // Add the current command to history
        (*history)[*history_index] = strdup(user_in);
        *history_index = (*history_index + 1) % HISTORY_SIZE;
        *history_len = (*history_len < HISTORY_SIZE) ? *history_len + 1 : HISTORY_SIZE; 
    }
    return tokens;
}

// this function is used to create a new history array
char **create_history_array()
{
    char **history = malloc(20 * sizeof(char *));
    if (!history)
    {
        allocation_error();
    }
    return history;
}

// this function prints the history array
void print_history(char **history, int history_len, int history_index)
{
    int print_index = 0;
    int i = 1;
    if (history_len == HISTORY_SIZE)
        print_index = history_index;
    do
    {
        printf("%d : %s", i, history[print_index]);
        ++print_index;
        if (print_index == HISTORY_SIZE)
            print_index = 0;
        ++i;
    } while (history_index != print_index);
}

// this function is used to save the history to the '.history' file
void save_history(char **history, int history_len, int history_index)
{
    FILE *file = fopen(HISTORY_FILENAME, "w");
    if (file == NULL)
    {
        file_error(HISTORY_FILENAME);
        return;
    }

    int save_index = 0;
    int i = 1;
    if (history_len == HISTORY_SIZE)
        save_index = history_index;
    // this loop prints each line in history to a new line in the file
    do
    {
        fprintf(file, "%s", history[save_index]);
        ++save_index;
        if (save_index == HISTORY_SIZE)
            save_index = 0;
        ++i;
    } while (history_index != save_index);

    fclose(file);
}

// this function is used to read the history in from the '.history' file
int read_history(char **history){

    int history_len = 0;
    int history_index = 0;

    FILE *file = fopen(HISTORY_FILENAME, "r");
    if(file == NULL){
        return 0;
    }

    char line[BUFFER_SIZE];
    
    while(fgets(line, sizeof(line), file) != NULL){
        char *input_string = malloc(sizeof(char) * BUFFER_SIZE);
        strcpy(input_string, line);
        //printf("%s\n", input_string);
        history[history_len] = strdup(input_string);
        history_len++;
        history_index++;
        if(history_index == HISTORY_SIZE) history_index = 0;
    }
    fclose(file);
    return history_len;
}

// this function is used to parse a command into a number value between 1 and 20
int parseHistoryPosition(char *command)
{
    if(!command || !*command) 
    {
        to_few_args_err();
        return -1;
    }
    
    int history_value = -1;
    
    int first_digit = *command;

    if(!isdigit(first_digit)) 
    {
        parsing_int_error();
        return -1;
    }
    else first_digit -= '0';
    
    if(*(command+1)){
        int second_digit = *(command+1);

        if(!isdigit(second_digit))
        { 
            parsing_int_error();
            return -1;
        }
        else second_digit -= '0';


        history_value = first_digit * 10 + second_digit;

        if(history_value < 1 || history_value > 20) 
        {
            value_out_of_bounds_error();
            return -1;
        }
    }
    else
    {
        history_value = first_digit;
        if(history_value < 1) 
        {
            value_out_of_bounds_error();
            return -1;
        }
    }
    return history_value;
}
