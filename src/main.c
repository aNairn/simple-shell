#include <stdio.h>
#include <string.h>
#include "simpleshell.h"

int main(void)
{
    while(1)
    {
        display_prompt();
        
        const char * seperator = " \n\t";

        char * user_in = get_users_input();
       
        int valid_input = input_is_valid(user_in);
        if(input_is_valid(user_in) == -1) {
            end(1); 
            break;
        }
        else if(valid_input == 0) 
            continue;

        
        // should be changed to handle an array of all tokens entered
        char * token = strtok(user_in, seperator);
         
        if(!strcmp(token, "exit"))
        {
            end(0);
            break;
        }
    }
    return 0;
}
