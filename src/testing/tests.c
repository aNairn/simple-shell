#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "../simpleshell.h"

void testing_mode()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        fork_error();
    }
    else if (pid == 0)
    {
        char **history = create_history_array();
        int history_len = 0;
        int history_index = 0;

        struct Alias **aliases = create_alias_array();
        int aliases_len = 0;

        chdir(getenv("HOME"));

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
            "alias a b\n",
            "unalias a\n",
            "alias list ls\n",
            "alias\n",
            "alias list dir\n"
            "alias\n",
            "list\n",
            "alias name\n",
            "unalias\n",
            "unalias fail this\n",
            "alias\n",
            "history\n"
        };
        int test_no = sizeof(tests) / sizeof(tests[0]);

        printf("======================================\n");
        printf("===========STARTING TESTING===========\n");
        printf("======================================\n");

        for (int i = 0; i < test_no; i++)
        {

            char *user_in;
            char *cwd = get_cwd();

            printf("\n------------------------------------\n");
            printf(">>> ------ TESTING : %s", tests[i]);
            printf("------------------------------------\n");

            display_prompt(cwd);
            printf("\n");
            user_in = tests[i];
            
            if(run(user_in, history, &history_len, &history_index, &aliases, &aliases_len)) break;

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
