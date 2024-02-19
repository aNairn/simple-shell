#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "simpleshell.h"
#include "testing/tests.h"
#include "error.h"

#define BUFFER_SIZE 1024
#define INPUT_LIMIT 512
#define TOKEN_DELIM " \t\n|><&;"


// functions



char * get_cwd()
{  
    char cwd[BUFFER_SIZE];
    char * cwd_p = cwd;

    getcwd(cwd, sizeof(cwd));
    
    return cwd_p;
}

void display_prompt(char * cwd)
{
    printf("%s:$>", cwd);
}

char * get_users_input()
{
    char buffer[BUFFER_SIZE];
    char * user_in;
    
    user_in = fgets(buffer, BUFFER_SIZE, stdin);

    return user_in;
}

char ** get_tokens(char * input)
{
    int i = 0;
    int buff = 16;
    char ** tokens = malloc(buff * sizeof(char *));
    char * token;
    
    if(!tokens)
    {
        perror("allocation error");
    }
    
    token = strtok(input, TOKEN_DELIM);

    while (token != NULL)
    {
        tokens[i] = token;
        i++;
        if(i>buff) {
            buff*=2;
            tokens = realloc(tokens, buff * sizeof(char *));
            if(!tokens){
                perror("<Allocation Error>");
            }
        }
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[i] = NULL;
    return tokens;
}

void change_dir(char ** tokens)
{  
    if(*tokens == NULL){
        chdir(getenv("HOME"));
    }
    else if(*(tokens+1) != NULL)
    {
        to_many_args_err();
    }
    else
    {
        if (!strcmp(*tokens, "~") || !strcmp(*tokens, "HOME"))
        {
            chdir(getenv("HOME"));
        }
        else if (chdir(*tokens) != 0)
        {
            struct stat buf;
            stat(*tokens,  &buf);
            FILE * f = fopen(*tokens, "r");
            if(f != NULL)
            {
                fclose(f);
                cd_into_file_error(tokens);
            }else 
            {
                not_valid_dir(tokens);
            }
        }
    }
}

void change_home(char ** tokens)
{
    if(*tokens == NULL)
    {
        to_few_args_err();
    }
    else if(*(tokens+1) != NULL)
    {
        to_many_args_err();
    }
    else
    {
        struct stat dir_stat;
        if(stat(*tokens,&dir_stat) != 0 || !S_ISDIR(dir_stat.st_mode)){
            perror("Directory does not exist");
        }
        else if(setenv("HOME", *tokens, 1) != 0){
            perror("change_home error");
        }
        else
        {
        printf("NEW HOME SET : %s\n", *tokens);
        }
    }
}

void change_path(char ** tokens)
{
    if(*tokens == NULL)
    {
        to_few_args_err();
    }
    else if(*(tokens+1) != NULL)
    {
        to_many_args_err();
    }
    else
    {
        struct stat dir_stat;
        if(stat(*tokens,&dir_stat) != 0 || !S_ISDIR(dir_stat.st_mode)){
            perror("Directory does not exist");
        }
        else if(setenv("PATH", *tokens, 1) != 0){
            perror("change_path error");
            }
            else
            {
            printf("NEW PATH SET : %s\n", *tokens);
            }
    }
}

void print_home()
{
    printf("%s\n", getenv("HOME"));
}

void print_path()
{
    printf("%s\n", getenv("PATH"));
}

void run_fork(char ** tokens)
{
    pid_t pid = fork();
            
    if(pid < 0)
    {
        fork_error();
    } 
    else if (pid == 0)
    {
        execvp(tokens[0], tokens);
        perror(tokens[0]);
        exit(1);
    } 
    else 
    {
        wait(NULL);
    }
}

void reset_env(char * starting_dir, char * starting_HOME, char * starting_PATH)
{
    chdir(starting_dir);
    setenv("HOME", starting_HOME, 1);
    setenv("PATH", starting_PATH, 1);
    printf("RESETING ENVIRO : \n\tHOME-%s \n\tPATH-%s\n", getenv("HOME"), getenv("PATH"));
    printf("Exiting...\n");
}

int parseCommand(char * command)
{    
    if(*(command+2))
    {
        return -1;
    }
    if(!*command)
    {
        return -1;
    }        
    if((*command < '0' || *command > '9'))
    {
        return -1;
    }
    if(*(command+1))
    {
        if( *command < '0' || *command > '9' )
        {
            return -1;
        }            
        if( *(command+1) < '0' || *(command+1)>'9' )
        {
            return -1;
        }
    }

    int history_value = 0;
    if(!*(command+1))
    {
        history_value = (*command)-48;
    }
    else
    {
        history_value = ((*command)-48)*10;
        history_value += ((*command)-48);
    }
    return history_value;
}

char ** create_history_array()
{
    char ** history = malloc(20*sizeof(char *));
    if(!history)
    {
        perror("<Allocation Error>");
    }
    return history;
}

void print_history(char ** history, int history_len)
{
    for(int i = 0; i < history_len; i++){
        printf("%d : %s", history_len-i, history[i]);
    }
}

void print_tokens(char ** tokens)
{
    while (*tokens != NULL)
    {    
        printf("\"%s\"\n", *tokens);
        tokens++;
    }
}

Alias ** create_alias_array()
{
    Alias ** aliases = malloc(10*sizeof(Alias*));
    if(!aliases)
    {
        perror("<Allocation Error>");
    }
    return aliases;
} 

Alias * create_alias(char * name, char * command)
{
    Alias * alias = malloc(sizeof(Alias));
    char ** tokens = get_tokens(command);
    alias->name = name;
    alias->commamd_tokens = tokens;
    return alias;
}

void add_alias(Alias ** aliases, Alias * alias)
{
    int len = sizeof(aliases)/sizeof(aliases[0]);
    if(!aliases+len)
    {
        aliases = realloc(aliases, 2*sizeof(aliases));
        if(!aliases)
        {
            perror("<Allocation Error>");
        }
    }
    int i = 0;
    while(aliases+i) ++i;
    aliases[i] = alias; 
}

void print_aliases(Alias ** aliases)
{
    while(*aliases != NULL)
    {
        printf("{ %s : ", (*aliases)->name);
        char ** tokens = (*aliases)->commamd_tokens;
        while (*tokens != NULL)
        {    
            printf("\"%s\" ", *tokens);
            ++tokens;
        }
        printf("}\n");
        aliases++;
    }
}
