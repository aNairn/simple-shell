#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simpleshell.h"

#define BUFFER_SIZE 1024
#define INPUT_LIMIT 512

// functions

// update this to get user details? or CWD?
void display_prompt()
{
    printf("simple shell:$>");
}

char * get_users_input()
{
    char buffer[BUFFER_SIZE];
    char * user_in;
    
    user_in = fgets(buffer, BUFFER_SIZE, stdin);

    return user_in;
}

void end(int newline)
{
    if(newline)
        printf("\nExiting...\n");
    else
        printf("Exiting...\n");
}


// error handling

void input_too_long_error()
{
    printf("<Input exceeds limit>\n");
}

int input_is_valid(char * input)
{
    if(input == NULL) 
    {
        return -1;
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
