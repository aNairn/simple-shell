#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "simpleshell.h"

int main(void){
    
    char * starting_dir = get_cwd();
    char * starting_PATH = getenv("PATH");
    char * starting_HOME = getenv("HOME");
    
    while(1){
    
        char * cwd = get_cwd();
        display_prompt(cwd);

        char * user_in;
        char ** tokens;

        user_in = get_users_input();

        int valid_input = input_is_valid(user_in);
        if(valid_input == -1) 
        {
            break;
        }
        else if(valid_input == 0) 
        {
            continue;
        }        
        
        tokens = get_tokens(user_in);
        if(*tokens == NULL){
            continue;
        }
        
        //-------uncomment to test tokens-------        
        // test_tokens(tokens);
        //--------------------------------------
       

        if(!strcmp(*tokens, "exit"))
        {
            break;
        }
        else if(!strcmp(*tokens, "cd"))
        {
            change_dir(tokens+1);
        }
        else if(!strcmp(*tokens, "set-home"))
        {
            change_home(tokens+1);
        }
        else if(!strcmp(*tokens, "print-home"))
        {
            printf("%s\n", getenv("HOME"));
        }
        else if(!strcmp(*tokens, "set-path"))
        {
            change_path(tokens+1);
        }
        else if(!strcmp(*tokens, "print-path"))
        {
            printf("%s\n", getenv("PATH"));
        }
        else 
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

    }

    chdir(starting_dir);
    setenv("HOME", starting_HOME, 1);
    setenv("PATH", starting_PATH, 1);
    printf("RESETING ENVIRO : \n\tHOME-%s \n\tPATH-%s\n", getenv("HOME"), getenv("PATH"));
    printf("Exiting...\n");
    return 0;
}
