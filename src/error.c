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
    fprintf(stderr, "<Input does not have enough arguments> : command takes more arguments\n");
}

void input_too_long_error()
{
    fprintf(stderr, "<Input exceeds limit> : reduce input\n");
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
    fprintf(stderr, "<New process error> : error starting new process\n");
}

void to_many_args_err()
{
    fprintf(stderr, "<To Many Arguments> : command takes less argumensts\n");
}

void not_valid_dir(char **tokens)
{
    fprintf(stderr, "<Directory '%s' does not exists>\n", *tokens);
}

void cd_into_file_error(char ** tokens)
{
    fprintf(stderr, "<Can't cd into file> : cd only works with directories\n");
}

void empty_history_error()
{
    fprintf(stderr, "<History Empty> : nothing to invoke\n");
}

void parsing_int_error()
{
    fprintf(stderr, "<Enter an Integer value> : only numbers can be entered for history commands\n");
}

void value_out_of_bounds_error()
{
    fprintf(stderr, "<Value entered out of range> : history commands can only be in the range 1 to 20\n");
}

void aliases_full_err()
{
    fprintf(stderr, "<Aliases Full> : Try removing unwanted aliases\n");
}

void no_alias_found_err()
{
    fprintf(stderr, "<No Alias Found> : try creating the alias first\n");
}

void alias_override_msg(char *name)
{
    fprintf(stderr, "<Alias '%s' updated>\n", name);
}   

void file_error(char *file_name)
{
    fprintf(stderr, "<Error opening file '%s'>\n", file_name);
}

void allocation_error(){
    fprintf(stderr, "<Error allocating memory> : fatal error allocating memory\n");
}