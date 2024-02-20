#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "simpleshell.h"
#include "error.h"
#include "testing/tests.h"

int main(void)
{
    char *starting_dir = get_cwd();
    char *starting_PATH = getenv("PATH");
    char *starting_HOME = getenv("HOME");

    char **history = create_history_array();
    int history_len = 0;

    struct Alias **aliases = create_alias_array();
    int aliases_len = 0;

    chdir(getenv("HOME"));

    while (1)
    {
        char **tokens;

        char *user_in;
        char *cwd = get_cwd();

        display_prompt(cwd);

        user_in = get_users_input();

        int valid_input = input_is_valid(user_in);

        if (valid_input == -1)
        {
            break;
        }
        else if (valid_input == 0)
        {
            continue;
        }

        tokens = get_tokens(strdup(user_in));

        // ====== CHECKING HISTORY ========
        if (*tokens == NULL)
        {
            continue;
        }
        else if (*tokens[0] == '!' && !*(tokens + 1))
        {
            if (history_len < 1)
            {
                empty_history_error();
                free(tokens);
                continue;
            }

            char *command = *tokens + 1;

            if (*command == '!')
            {
                command++;

                if (*command != '\0')
                {
                    to_many_args_err();
                    free(tokens);
                    continue;
                }
                else
                {
                    tokens = get_tokens(strdup(history[history_len - 1]));
                }
            }
            else
            {
                if (*command == '-')
                {
                    command++;
                }

                int history_index = parseCommand(command);
                if (history_index < 0)
                {
                    parsing_int_error();
                    free(tokens);
                    continue;
                }
                else if (history_index < 1 || history_index > 20)
                {
                    value_out_of_bounds_error();
                    free(tokens);
                    continue;
                }
                else
                {
                    tokens = get_tokens(strdup(history[history_len - history_index]));
                }
            }
        }
        else if (**tokens == '!' && *(tokens + 1))
        {
            to_many_args_err();
            free(tokens);
            continue;
        }
        else
        {
            history[history_len] = strdup(user_in);
            history_len++;
        }

        // ====== CHECKING ALIASES ========
        Alias * existing_alias;
        if((existing_alias = alias_exists(aliases, *tokens)))
        {
            tokens = get_alias_command(existing_alias, tokens);
        }

        if (!strcmp(*tokens, "alias"))
        {
            if (*(tokens + 1) == NULL)
            {
                print_aliases(aliases);
                free(tokens);
                continue;
            }
            else
            {
                char *name = *(tokens + 1);
                char **command = tokens + 2;

                if (*command == NULL)
                {
                    to_few_args_err();
                    continue;
                }

                Alias *existing_alias;
                if((existing_alias = alias_exists(aliases, name)))
                {
                    existing_alias->command_tokens = command;
                    continue;
                }

                Alias *alias = create_alias(name, command);

                if (add_alias(aliases, alias, aliases_len))
                {
                    ++aliases_len;
                }
                continue;
            }
        }
        else if (!strcmp(*tokens, "unalias"))
        {
            tokens++;
            if(!*tokens)
            {
                to_few_args_err();
                continue;
            }
            else if(*(tokens+1))
            {
                to_many_args_err();
                continue;
            }
            char *name = *tokens;
            if(alias_exists(aliases, name))
            {
                aliases = remove_alias(aliases, name);
                aliases_len--;
            }
            else
            {
                no_alias_found_err();
            }
            continue;
        }

        // ====== RUNNING COMMANDS ========
        if (!strcmp(*tokens, "exit"))
        {
            free(tokens);
            break;
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
        else if (!strcmp(*tokens, "restorepath"))
        {
            change_path(&starting_PATH);
        }
        else if (!strcmp(*tokens, "history"))
        {
            print_history(history, history_len);
        }
        else
        {
            run_fork(tokens);
        }

        free(tokens);
    }

    reset_env(starting_dir, starting_HOME, starting_PATH);
    free(history);

    return 0;
}
