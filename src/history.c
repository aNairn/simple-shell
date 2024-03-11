#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "simpleshell.h"

char **create_history_array()
{
    char **history = malloc(20 * sizeof(char *));
    if (!history)
    {
        perror("<Allocation Error>");
    }
    return history;
}

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
