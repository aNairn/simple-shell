#include <stdio.h>
#include <string.h>
#include "simpleshell.h"

int main(void){
    while(1){
        
        display_prompt();
        
        const char seperator[2] = " ";
        char user_in[1024];

        char * user_in_p = user_in;
        char * token;

        scanf("%s", user_in_p);

        token = strtok(user_in_p, seperator);

        if(strlen(token) > 512)
        {
            input_too_long_error();
            continue;
        }



        // exit function only working when the string is 'exit ' not 'exit' [note white space]
        if(!strcmp(token, "exit"))
        {
            printf("Exiting...\n");
            break;
        }

    }

    return 0;
}