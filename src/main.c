#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "simpleshell.h"

int main(void){
    
    while(1){
        
        display_prompt();

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
        test_tokens(tokens);
        //--------------------------------------

        if(!strcmp(*tokens, "exit"))
        {
            break;
        }
        else {
            pid_t pid = fork();
            if(pid < 0){
                fork_error();
            } else if (pid == 0){
                execvp(tokens[0], tokens);
                perror(tokens[0]);
                exit(1);
            } else {
                wait(NULL);
            }

        }

    }
    printf("Exiting...\n");
    return 0;
}
