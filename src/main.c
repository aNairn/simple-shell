#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "simpleshell.h"

void splash_screen()
{
    printf("-------------------------------------------------------------------------------------\n---                                888             888888            _.---._      ---\n--                                 888             888888        __.'"
           ".'/|\\`."
           "'.__   --\n--                                 888             888888       :__.' / | \\ `.__:  --\n-- .d8888b  .d88b.  8888b. .d8888b 88888b.  .d88b. 888888       '.'  /  |  \\  `.'  --\n-- 88K     d8P  Y8b    \"88b88K     888 \"88bd8P  Y8b888888        `. /   |   \\ .'   --\n-- \"Y8888b.88888888.d888888\"Y8888b.888  88888888888888888          `-.__|__.-'     --\n--      X88Y8b.    888  888     X88888  888Y8b.    888888                          --\n--- 88888P' \"Y8888 \"Y888888 88888P'888  888 \"Y8888 888888                         ---\n-------------------------------------------------------------------------------------\n\n");
}

int main(void)
{
    splash_screen();

    chdir(getenv("HOME"));

    char *starting_dir = get_cwd();
    char *starting_PATH = getenv("PATH");
    char *starting_HOME = getenv("HOME");

    struct Alias **aliases = create_alias_array();
    int aliases_len = read_aliases(aliases);

    char **history = create_history_array();
    int history_len = read_history(history);
    int history_index = 0;
    if (history_len < HISTORY_SIZE)
        history_index = history_len;

    while (1)
    {
        char *cwd = get_cwd();
        display_prompt(cwd);

        char *user_in;
        user_in = get_users_input();

        if (run(user_in, history, &history_len, &history_index, &aliases, &aliases_len))
            break;
    }

    reset_env(starting_dir, starting_HOME, starting_PATH);
    chdir(starting_HOME);
    save_history(history, history_len, history_index);
    save_aliases(aliases, aliases_len);
    free(aliases);
    free(history);

    return 0;
}

int run(char *user_in, char **history, int *history_len, int *history_index, Alias ***aliases, int *aliases_len)
{
    char **tokens;

    int valid_input = input_is_valid(user_in);
    if (valid_input == -1)
    {
        return 1;
    }
    else if (valid_input == 0)
    {
        return 0;
    }

    tokens = get_tokens(strdup(user_in));

    // need to somehow loop here in order to constantly update the tokens
    // untill nothing to change

    // ====== CHECKING ALIASES ========
    int check_done = 0;
    char **checked = malloc(5 * sizeof(char *));
    int check_num = 0;
    do
    {
        Alias *existing_alias;

        if ((existing_alias = alias_exists(*aliases, *tokens, *aliases_len)))
        {
            for(int i = 0; i <= check_num; i++){
                if(!strcmp(checked[i], existing_alias->name)){
                    printf("<Alias loop detected breaking>");
                    check_done = 1;
                    break;
                }
            }
            checked[check_num] = existing_alias->name;
            tokens = get_alias_command(existing_alias, tokens);
        }
        if (!strcmp(*tokens, "alias"))
        {
            if (*(tokens + 1) == NULL)
            {
                print_aliases(*aliases, *aliases_len);
                free(tokens);
                return 0;
            }
            else
            {
                char *name = *(tokens + 1);
                char **command = tokens + 2;

                // Make sure tokens are valid
                if (*command == NULL)
                {
                    to_few_args_err();
                    return 0;
                }

                Alias *existing_alias;
                if ((existing_alias = alias_exists(*aliases, name, *aliases_len)))
                {
                    existing_alias->command_tokens = command;
                    alias_override_msg(name);
                    return 0;
                }

                Alias *alias = create_alias(name, command);
                if (add_alias(*aliases, alias, *aliases_len))
                {
                    ++*aliases_len;
                }
                return 0;
            }
        }
        else if (!strcmp(*tokens, "unalias"))
        {
            tokens++;
            if (!*tokens)
            {
                to_few_args_err();
                return 0;
            }
            else if (*(tokens + 1))
            {
                to_many_args_err();
                return 0;
            }
            char *name = *tokens;
            if (alias_exists(*aliases, name, *aliases_len))
            {
                *aliases = remove_alias(*aliases, name);
                --*aliases_len;
            }
            else
            {
                no_alias_found_err();
            }
            return 0;
        }
        ++check_num;

    } while (!check_done && check_num < 5);
    
    // ====== CHECKING HISTORY ========
    if (*tokens == NULL)
    {
        return 0;
    }
    else if (*tokens[0] == '!' && !*(tokens + 1))
    {
        if (*history_len < 1)
        {
            empty_history_error();
            free(tokens);
            return 0;
        }

        char *command = *tokens + 1;

        if (*command == '!')
        {
            command++;

            if (*command != '\0')
            {
                to_many_args_err();
                free(tokens);
                return 0;
            }
            else
            {
                tokens = get_tokens(strdup(history[(*history_index - 1 + HISTORY_SIZE) % HISTORY_SIZE]));
            }
        }
        else
        {
            if (*command == '-')
            {
                command++;
            }

            int history_value = parseCommand(command);

            if (history_value < 0)
            {
                parsing_int_error();
                free(tokens);
                return 0;
            }
            else if (history_value < 1 || history_value > HISTORY_SIZE || history_value > *history_len)
            {
                value_out_of_bounds_error();
                free(tokens);
                return 0;
            }
            else
            {
                tokens = get_tokens(strdup(history[(*history_index - history_value + *history_len) % *history_len]));
            }
        }
    }
    else if (**tokens == '!' && *(tokens + 1))
    {
        to_many_args_err();
        free(tokens);
        return 0;
    }
    else
    {
        // Add the current command to history
        history[*history_index] = strdup(user_in);
        *history_index = (*history_index + 1) % HISTORY_SIZE;
        *history_len = (*history_len < HISTORY_SIZE) ? *history_len + 1 : HISTORY_SIZE;
    }

    // ====== RUNNING COMMANDS ========
    if (!strcmp(*tokens, "exit"))
    {
        free(tokens);
        return 1;
    }
    else if (!strcmp(*tokens, "test"))
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
        print_home();
    }
    else if (!strcmp(*tokens, "setpath"))
    {
        change_path(tokens + 1);
    }
    else if (!strcmp(*tokens, "getpath"))
    {
        print_path();
    }
    else if (!strcmp(*tokens, "history"))
    {
        print_history(history, *history_len, *history_index);
    }
    else
    {
        run_fork(tokens);
    }

    free(tokens);
    return 0;
}
