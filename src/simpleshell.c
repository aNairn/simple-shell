#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include "simpleshell.h"

#define BUFFER_SIZE 1024
#define INPUT_LIMIT 512
#define TOKEN_DELIM " \t\n|><&;"


// functions

// update this to get user details? or CWD?
char * get_cwd(){
    
    char cwd[BUFFER_SIZE];
    char * cwd_p = cwd;

    getcwd(cwd, sizeof(cwd));
    
    return cwd_p;
}

void display_prompt(char * cwd){

    printf("%s:$>", cwd);
    
}

char * get_users_input(){

    char buffer[BUFFER_SIZE];
    char * user_in;
    
    user_in = fgets(buffer, BUFFER_SIZE, stdin);

    return user_in;
}

char ** get_tokens(char * input){
    
    int i = 0;
    int buff = 16;

    char ** tokens = malloc(buff * sizeof(char *));
    
    char * token;
    


    if(!tokens){
        perror("allocation error");
    }
    token = strtok(input, TOKEN_DELIM);

    
    while (token != NULL){
        tokens[i] = token;
        i++;
        if(i>buff) {
            buff*=2;
            tokens = realloc(tokens, buff * sizeof(char *));
            if(!tokens){
                perror("allocation error");
            }
        }
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[i] = NULL;
    return tokens;
}

void change_dir(char ** tokens){
    
    if(*(tokens+1) != NULL)
    {
        to_many_args_err();
    }
    else
    {
        if(!strcmp(*tokens, "~") || !strcmp(*tokens, "HOME"))
        {
            chdir(getenv("HOME"));
        }
        else
        {
            chdir(*tokens);
        }
    }
}

void change_home(char ** tokens){
    if(*(tokens+1) != NULL){
        to_many_args_err();
    }
    else
    {
        setenv("HOME", *tokens, 1);
        printf("NEW HOME SET : %s\n", *tokens);
    }
}

void change_path(char ** tokens){
    if(*(tokens+1) != NULL){
        to_many_args_err();
    }
    else
    {
        setenv("PATH", *tokens, 1);
        printf("NEW PATH SET : %s\n", *tokens);
    }
}

// error handling

void input_too_long_error(){

    printf("<Input exceeds limit>\n");

}

int input_is_valid(char * input){

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

void fork_error(){
    perror("New process error");
}

void to_many_args_err(){
    perror("To Many Arguments");
}
// test functions

void test_tokens(char ** tokens){

    while (*tokens != NULL){
        
        printf("\"%s\"\n", *tokens);
        tokens++;

    }

}
