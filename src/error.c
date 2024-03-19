#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "simpleshell.h" 

// error handling
void to_few_args_err()
{
    perror("<Input does not have enough arguments>");
}

void input_too_long_error()
{
    perror("<Input exceeds limit>");
}

int input_is_valid(char * input)
{
    if(input == NULL) 
    {
        printf("\n");
        return -1;
    }
    else if(*input == '\n') 
    {
        return 0;
    }
    else if(strlen(input) > INPUT_LIMIT)
    {
        input_too_long_error();
        return 0;
    }
    else 
    {
        return 1;
    }
}

void fork_error()
{
    perror("<New process error>");
}

void to_many_args_err()
{
    perror("<To Many Arguments>");
}

void not_valid_dir(char **tokens)
{
    fprintf(stderr, "<Directory '%s' does not exists>\n", *tokens);
}

void cd_into_file_error(char ** tokens)
{
    perror("<Can't cd into file>");
}

void empty_history_error()
{
    perror("<History Empty>");
}

void parsing_int_error()
{
    perror("<Enter an Integer value>");
}

void value_out_of_bounds_error()
{
    perror("<Value entered out of range>");
}

void aliases_full_err()
{
    perror("<Aliases Full : Try removing unwanted aliases>");
}

void no_alias_found_err()
{
    perror("<No Alias Found>");
}

void alias_override_msg(char *name)
{
    fprintf(stderr, "<Alias '%s' updated>\n", name);
}   

void file_error(char *file_name)
{
    fprintf(stderr, "<Error opening file '%s'>", file_name);
}