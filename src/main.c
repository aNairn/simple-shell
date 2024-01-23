#include <stdio.h>
#include <string.h>
#include "simpleshell.h"

int main(void){
    
    while(1){
        
        display_prompt();
        
        const char * seperator = " \n\t";

        char * user_in = get_users_input();
        
        if(user_in == NULL)
        {
            end();
            break;
        }

        char * token = strtok(user_in, seperator);
        
        if(!token_is_valid(token)) continue;
       
        if(!strcmp(token, "exit"))
        {
            end();
            break;
        }

    }

    return 0;
}
