#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <ctype.h>

#include "simpleshell.h"

// this function is used to create a new history array
char **create_history_array()
{
    char **history = malloc(20 * sizeof(char *));
    if (!history)
    {
        perror("<Allocation Error>");
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
    if(!command || !*command) return -1;
    
    int history_value = -1;
    
    int first_digit = *command;

    if(!isdigit(first_digit)) return -1;
    else first_digit -= '0';
    
    if(*(command+1)){
        int second_digit = *(command+1);

        if(!isdigit(second_digit)) return -1;
        else second_digit -= '0';


        history_value = first_digit * 10 + second_digit;

        if(history_value < 1 || history_value > 20) return -1;
    }
    else
    {
        history_value = first_digit;
        if(history_value < 1) return -1;
    }
    return history_value;
}
