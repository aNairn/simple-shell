#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "simpleshell.h"

int main(void){
    char * starting_dir = get_cwd();
    char * starting_PATH = getenv("PATH");
    char * starting_HOME = getenv("HOME");
    
    chdir(getenv("HOME"));

    while(1)
    {
        char * cwd = get_cwd();
        char * user_in;
        char ** tokens;

        display_prompt(cwd);

        user_in = get_users_input();
        
        //TODO: HISTORY CHECK HERE

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
        else if(!strcmp(*tokens, "sethome"))
        {
            change_home(tokens+1);
        }
        else if(!strcmp(*tokens, "gethome"))
        {
            print_home();
        }
        else if(!strcmp(*tokens, "setpath"))
        {
            change_path(tokens+1);
        }
        else if(!strcmp(*tokens, "getpath"))
        {
            print_path();
        }
        else if(!strcmp(*tokens, "restorepath"))
        {
            change_path(&starting_PATH);
        }
        else 
        {
            run_fork(tokens);
        }
    }

    reset_env(starting_dir, starting_HOME, starting_PATH);

    return 0;
}
