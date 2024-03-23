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
    // Aliases will be checked a maximum of 10 times ensuring every alias can be checked at least onece
    // any more than that and a loop is detected
    int check_num = 0;
    do
    {
        // checj to see if the current input matches an existing alias an if it does set the tokens 
        // to match the alias tokens
        Alias *existing_alias;
        if ((existing_alias = alias_exists(*aliases, *tokens, *aliases_len)))
        {
            tokens = get_alias_command(existing_alias, tokens);
        }

        // If 'alias' is detected this block works out how to process the instruction
        if (!strcmp(*tokens, "alias"))
        {
            // if "ONLY" alias is entered print the aliases 
            if (*(tokens + 1) == NULL)
            {
                print_aliases(*aliases, *aliases_len);
                return 0;
            }
            else // check the perameters are valid and create the new alias
            {
                // get the name of the alias and the command string
                char *name = *(tokens + 1);
                char **command = tokens + 2;

                // Make sure command tokens are valid
                if (*command == NULL)
                {
                    to_few_args_err();
                    return 0;
                }

                // Check if the alias being entered exists and if it does override the existing
                // con=mmand with the new command
                Alias *existing_alias;
                if ((existing_alias = alias_exists(*aliases, name, *aliases_len)))
                {
                    update_alias(*aliases, existing_alias, command, *aliases_len);
                    free_tokens(tokens);
                    return 0;
                }

                // if the name and command are valid create a new alias and add it to the alias array
                // print_aliases(*aliases, *aliases_len);
                Alias *alias = create_alias(name, command);
                if (add_alias(*aliases, alias, *aliases_len))
                {
                    ++(*aliases_len);
                }
                
                return 0;
            }
        }
        // if the unalias command is entered check the paramaters are valid and handle the instruction
        else if (!strcmp(*tokens, "unalias"))
        {
            tokens++;
            // if there are no parameters show an error
            if (!*tokens)
            {
                to_few_args_err();
                return 0;
            }
            // if there are more than one parameters entered then there are to many 
            //  so show an appropriate error
            else if (*(tokens + 1))
            {
                to_many_args_err();
                return 0;
            }
            // if the alias to be remove exists then remove correctly
            if (alias_exists(*aliases, *tokens, *aliases_len))
            {
                *aliases = remove_alias(*aliases, *tokens, aliases_len);
            }
            else // If the alias to be removed doesnt exist show an appropriate message
            {
                no_alias_found_err();
            }
            return 0;
        }
        ++check_num;
    } while (check_num < 10);

    // ====== CHECKING HISTORY ========
    // Handle requests to read from history.
    // check that the first token starts with '!' and there are no other tokens
    if (*tokens[0] == '!' && !*(tokens + 1))
    {
        // if the history is empty then show an appropriate error message and free the tokens.
        if (*history_len < 1)
        {
            empty_history_error();
            free_tokens(tokens);
            return 0;
        }

        // get the second character of the first token if it is anothe '!' and there are no more
        // characters execute the most recent command in history
        char *command = *tokens + 1;
        if (*command == '!')
        {
            command++;

            // if there are more characters throw an appropriate error
            if (*command != '\0')
            {
                to_many_args_err();
                free_tokens(tokens);
                return 0;
            }
            else
            {
                tokens = get_tokens(strdup(history[(*history_index - 1 + HISTORY_SIZE) % HISTORY_SIZE]));
            }
        }
        else // if the next character ISN'T '!' decide how to handle it checking for digits or a '-'
        {
            // if the next character is '-' set the latest flag to true and move on
            int latest = 0;
            if (*command == '-')
            {
                command++; 
                latest = 1;           
            }

            // parse the remender of the token into a number value from 1 to 20 to be used to find the
            // correct command in history
            int history_value = parseHistoryPosition(command);
            // if the value comes back less than zero there has been a parsing error show an appropriate
            // message the free the tokens and continue
            if (history_value < 0)
            {
                parsing_int_error();
                free_tokens(tokens);
                return 0;
            }
            // if the value returned is out of the desired range through an out of bounds error
            else if (history_value < 1 || history_value > HISTORY_SIZE || history_value > *history_len)
            {
                value_out_of_bounds_error();
                free_tokens(tokens);
                return 0;
            }
            else // if the position parsed is valid then decide if it is latest or oldest command
            {
                // of the latest flag is true get the history starting from the most recent else
                // get it starting from the oldest
                if(latest){
                    tokens = get_tokens(strdup(history[(*history_index - history_value + *history_len) % *history_len]));
                }
                else{

                    tokens = get_tokens(strdup(history[((*history_index - 1) +  history_value) % *history_len]));
                }
            }
        }
    }
    // if there is to many parameters then show an appropriate message
    else if (**tokens == '!' && *(tokens + 1))
    {
        to_many_args_err();
        free_tokens(tokens);
        return 0;
    }
    // if there is no history call then save the entered command into history.
    else
    {
        // Add the current command to history
        history[*history_index] = strdup(user_in);
        *history_index = (*history_index + 1) % HISTORY_SIZE;
        *history_len = (*history_len < HISTORY_SIZE) ? *history_len + 1 : HISTORY_SIZE;
    }

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
