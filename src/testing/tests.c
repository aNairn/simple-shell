#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../simpleshell.h"

void testing_mode()
{
    // create a fork for the tests to run on
    pid_t pid = fork();

    if (pid < 0)
    {
        fork_error();
    }
    else if (pid == 0)
    {
        // prepare the test enviroment
        struct Alias **aliases = create_alias_array();
        int aliases_len = 0;
        char **history = create_history_array();
        int history_len = 0;
        int history_index = 0;
        
        chdir(getenv("HOME"));

        // this array hold all of the test commands
        char *tests[] = {
            "ls\t-lF;.&..>.<..|/\tfksdk\n", 
            "gethome\n",
            "sethome /mnt/c/Users\n",
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
            "Testing this\n",
            "alias\n",
            "alias a b\n",
            "alias\n",
            "unalias a\n",
            "alias list ls\n",
            "alias\n",
            "alias list dir\n",
            "alias\n",
            "list\n",
            "alias name\n",
            "unalias\n",
            "unalias fail this\n",
            "alias\n",
            "history\n",
            "history\n",
            "history\n",
            "history\n",
            "history\n",
            "history\n",
            "history\n",
        };
        int test_no = sizeof(tests) / sizeof(tests[0]);

        printf("======================================\n");
        printf("===========STARTING TESTING===========\n");
        printf("======================================\n");

        // loop the tests array and run each test
        for (int i = 0; i < test_no; i++)
        {

            char *user_in;
            char *cwd = get_cwd();

            printf("\n------------------------------------\n");
            printf(">>> ------ TESTING : %s", tests[i]);
            printf("------------------------------------\n");

            display_prompt(cwd);
            printf("\n");
            // get the current test and set it as the user input and ensure it is valid
            user_in = tests[i];
            int valid_input = input_is_valid(user_in);
            if (valid_input == -1)
            {
                printf("invalid input on test [%d] : '%s'", i, user_in);
                break;
            }
            else if (valid_input == 0)
            {
                continue;
            }
            else
            {
                // run the test command
                if (run(user_in, history, &history_len, &history_index, &aliases, &aliases_len))
                {
                    printf("invalid input on test [%d] : '%s'", i, user_in);
                    break;
                }
            }
            printf("-----------------------------\n");
            printf("<<< ------ SUCCESS ------ >>>\n\n");
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
