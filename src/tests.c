#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "simpleshell.h"
#include "error.h"
#include "tests.h"

void testing_mode(){

    pid_t pid = fork();
            
    if(pid < 0)
    {
        fork_error();
    } 
    else if (pid == 0)
    {
        
        char * starting_PATH = getenv("PATH");
        
        char ** history = malloc(20*sizeof(char *));

        int history_len = 0;
        int history_index = 0;
        int HISTORY_SIZE = 20;
        
        chdir(getenv("HOME"));

        char * tests[] = {
            "ls\t-lF;.&..>.<..|/\tfksdk\n",
            "gethome\n",
            "sethome /mnt/c/Users/PARKj\n",
            "gethome\n",
            "getpath\n",
            "setpath /bin\n",
            "getpath\n",
            "cd ..\n",
            "ls\n",
            "cd ~\n",
            "ls leetCode\n",
            "tokens these are all tokens\n",
            "history\n",
            "!!\n",
            "!1\n",
            "!-1\n",
            "Testing this\n"
        };
        int test_no = sizeof(tests)/sizeof(tests[0]);
        printf("======================================\n");
        printf("===========STARTING TESTING===========\n");
        printf("======================================\n");
        for(int i = 0; i < test_no; i++){
            
            char ** tokens;
            char * user_in;
            char * cwd = get_cwd();
            
            
            printf("\n------------------------------------\n");
            printf(">>> ------ TESTING : %s", tests[i]);
            printf("------------------------------------\n");

            display_prompt(cwd);

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
                    tokens = get_tokens(strdup(user_in));

                    // CHECKING HISTORY
                    if(*tokens == NULL)
                    {
                        continue;
                    }
                    else if(*tokens[0] == '!' && !*(tokens+1))
                    {
                        if(history_len < 1)
                        {
                            empty_history_error();
                            free(tokens);
                            continue;
                        }

                        char * command = *tokens+1;

                        if(*command == '!')
                        {
                            command++;

                            if(*command != '\0')
                            {
                                to_many_args_err();
                                free(tokens);
                                continue;
                            }
                            else
                            {
                                tokens = get_tokens(strdup(history[(history_index - 1 + HISTORY_SIZE) % HISTORY_SIZE]));
                            }
                        }
                        else
                        {
                            if(*command == '-')
                            {
                                command++;
                            }

                            int history_index = parseCommand(command);
                            if(history_index < 0)
                            {
                                parsing_int_error();
                                free(tokens);
                                continue;
                            }
                            else if(history_index < 1 || history_index > HISTORY_SIZE)
                            {
                                value_out_of_bounds_error();
                                free(tokens);
                                continue;
                            }
                            else
                            {
                                tokens = get_tokens(strdup(history[(history_index - 1 + HISTORY_SIZE) % HISTORY_SIZE]));
                            }
                        }
                    }
                    else if(**tokens == '!' && *(tokens+1))
                    {
                        to_many_args_err();
                        free(tokens);
                        continue;
                    }
                    else
                    {
                        // Add the current command to history
                        free(history[history_index]);
                        history[history_index] = strdup(user_in);
                        history_index = (history_index + 1) % HISTORY_SIZE;
                        history_len = (history_len < HISTORY_SIZE) ? history_len + 1 : HISTORY_SIZE;
                    }

                    // Make sure tokens are valid

                    if(!strcmp(*tokens, "exit"))
                    {
                        free(tokens);
                        break;
                    }
                    else if(!strcmp(*tokens, "test"))
                    {
                        if(*(tokens+1))
                        {
                            to_many_args_err();
                        }
                        else
                        {
                            testing_mode();
                        }
                    }
                    else if (!strcmp(*tokens, "tokens"))
                    {
                        print_tokens(tokens);
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
                    else if(!strcmp(*tokens, "history"))
                    {
                        print_history(history, history_len, history_index, HISTORY_SIZE);
                    }
                    else
                    {
                        run_fork(tokens);
                    }
            printf("-----------------------------\n");
            printf("<<< ------ SUCCESS ------ >>>\n\n");
            free(tokens);
        }
        printf("====================================\n");
        printf("===== TESTS COMPLETE  %d/%d  =======\n", test_no, test_no);
        printf("====================================\n");
        exit(1);
    } 
    else 
    {
        wait(NULL);
    }
}
