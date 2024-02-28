#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "simpleshell.h" 

#define INPUT_LIMIT 512

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
    printf("<Directory '%s' does not exists>\n", *tokens);
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
     char *msg_head = "<Alias '";
     char *msg_tail = "' updated>";
     char *msg = malloc(strlen(msg_head) + strlen(name) + strlen(msg_tail));
     strcat(msg, msg_head);
     strcat(msg, name);
     strcat(msg, msg_tail);
     perror(msg);
}   

void file_error(char *file_name)
{
    char *msg_frag = "<Error opening file ";
    int msg_len = strlen(msg_frag)+strlen(file_name)+3;
    char *msg = malloc(msg_len);
    strcat(msg, msg_frag);
    strcat(msg, "'");
    strcat(msg, file_name);
    strcat(msg, "'");
    strcat(msg, ">");
    perror(msg);
}