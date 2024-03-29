#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "simpleshell.h"

//splash screen to be shown at the start of the program
void splash_screen()
{
    printf("-------------------------------------------------------------------------------------\n---                                888             888888            _.---._      ---\n--                                 888             888888        __.'.'/|\\`.'.__   --\n--                                 888             888888       :__.' / | \\ `.__:  --\n-- .d8888b  .d88b.  8888b. .d8888b 88888b.  .d88b. 888888       '.'  /  |  \\  `.'  --\n-- 88K     d8P  Y8b    \"88b88K     888 \"88bd8P  Y8b888888        `. /   |   \\ .'   --\n-- \"Y8888b.88888888.d888888\"Y8888b.888  88888888888888888          `-.__|__.-'     --\n--      X88Y8b.    888  888     X88888  888Y8b.    888888                          --\n--- 88888P' \"Y8888 \"Y888888 88888P'888  888 \"Y8888 888888                         ---\n-------------------------------------------------------------------------------------\n\n");
}

// thie main run method where the program is initialized
int main(void)
{
    // prints the splashscreen display
    splash_screen();

    chdir(getenv("HOME"));

    // Store all the starting enviroments to be used in restore stage
    char *starting_dir = get_cwd();
    char *starting_PATH = getenv("PATH");
    char *starting_HOME = getenv("HOME");

    // Initialize the aliases array and read any saved aliases from the file
    struct Alias **aliases = create_alias_array();
    int aliases_len = read_aliases(aliases);

    // initialize the history array and read the history from a previous session and get lenght and index
    char **history = create_history_array();
    int history_len = read_history(history);
    int history_index = 0;
    if (history_len < HISTORY_SIZE)
        history_index = history_len;

    //set program as running to be used for main loop escape
    int running = 1;
    do
    {
        //display the current directory
        char *cwd = get_cwd();
        display_prompt(cwd);

        // get the users input    
        char *user_in;
        user_in = get_users_input();

        // ensure the input is a valid input
        int valid_input = input_is_valid(user_in);
        if (valid_input == -1)
        {
            running = 0;
        }
        else if (valid_input == 0)
        {
            continue;
        }
        else
        {
            // call the run method
            if (run(user_in, history, &history_len, &history_index, &aliases, &aliases_len))
                running = 0;
        }
    } while (running);

    // reset all enviroment to match the starting conditions
    reset_env(starting_dir, starting_HOME, starting_PATH);
    chdir(starting_HOME);
    // save the history and aliases to the appropriate files
    save_history(history, history_len, history_index);
    save_aliases(aliases, aliases_len);
    // free uo the alias and history arrays
    free_aliases(aliases, aliases_len);
    free(history);

    return 0;
}

// This method handles each input
int run(char *user_in, char **history, int *history_len, int *history_index, Alias ***aliases, int *aliases_len)
{
    // get the tokens from the users input
    char **tokens;
    tokens = get_tokens(strdup(user_in));

    // ====== CHECKING ALIASES ========
    check_aliases(aliases, &tokens, aliases_len);

    // ====== CHECKING HISTORY ========
    tokens = check_history(&history, history_len, history_index, tokens, user_in);
    if(tokens == NULL){
        free_tokens(tokens);
        return 0;
    }
    
    // ======= RECHECK ALIASES ======== 
    check_aliases(aliases, &tokens, aliases_len);

    // ====== RUNNING COMMANDS ========
    // take the users input and call the appropriate command
    if (!strcmp(*tokens, "exit"))
    {
        free_tokens(tokens);
        return 1;
    }
    else if (!strcmp(*tokens, "test")) // <------ run the test suite.
    {
        if (*(tokens + 1))
        {
            to_many_args_err();
        }
        else
        {
            testing_mode();
        }
    }
    else if (!strcmp(*tokens, "alias")) // If 'alias' is detected this block works out how to process the instruction
        {
            // if "ONLY" alias is entered print the aliases 
            if (*(tokens + 1) == NULL)
            {
                print_aliases(*aliases, *aliases_len);
            }
            else // check the perameters are valid and create the new alias
            {
                add_new_alias(aliases, tokens, aliases_len);
            }
        }
        else if (!strcmp(*tokens, "unalias")) // if the unalias command is entered check the paramaters are valid and handle the instruction
        {
            tokens++;
            unalias(aliases, tokens, aliases_len);
            return 0;
        }
    else if (!strcmp(*tokens, "tokens"))
    {
        print_tokens(tokens);
    }
    else if (!strcmp(*tokens, "cd"))
    {
        change_dir(tokens + 1);
    }
    else if (!strcmp(*tokens, "sethome"))
    {
        change_home(tokens + 1);
    }
    else if (!strcmp(*tokens, "gethome"))
    {
        if(*(tokens+1)) to_many_args_err();
        else print_home();
    }
    else if (!strcmp(*tokens, "setpath"))
    {
        change_path(tokens + 1);
    }
    else if (!strcmp(*tokens, "getpath"))
    {
        if(*(tokens+1)) to_many_args_err();
        else print_path();
    }
    else if (!strcmp(*tokens, "history"))
    {
        if(*(tokens+1))
            to_many_args_err();
        else
            print_history(history, *history_len, *history_index);
    }
    else
    {
        run_fork(tokens);
    }
    // free the tokens and finish the run
    free_tokens(tokens);
    return 0;
}
