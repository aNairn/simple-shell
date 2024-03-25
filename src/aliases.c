#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "simpleshell.h"

// This function is used to create a new alias array
Alias **create_alias_array()
{
    Alias **aliases = malloc(MAX_ALIASES * sizeof(Alias *));
    if (!aliases)
    {
        allocation_error();
    }
    int i = 0;
    while(aliases[i]){
        aliases[i] = NULL;
        ++i;
    }
    return aliases;
}

// This function checks to see if an alias exists in the aliases array
Alias *alias_exists(Alias **aliases, char *name, int aliases_len)
{
    for(int i = 0; i < aliases_len; i++){
        Alias *alias = aliases[i];
        if (!strcmp(alias->name, name))
        {
           return alias;
        }
    }
    return NULL;
}

// this function is used to create a new alias
Alias *create_alias(char *name, char **tokens)
{
    Alias *alias = malloc(sizeof(Alias));
    alias->name = name;
    alias->command_tokens = tokens;
    return alias;
}

// this finction is used to save the alias array to the '.aliases' file in the home directory
void save_aliases(Alias **aliases, int aliases_len)
{
    // create a new file and open it
    FILE *file;
    file = fopen(ALIASES_FILENAME, "w");
    if (file == NULL)
    {
        file_error(ALIASES_FILENAME);
        return;
    }
    for (int i = 0; i < aliases_len; i++)
    {
        // loop each alias and save it to a new line in the file
        int len = strlen(aliases[i]->name);
        char **alias_command = aliases[i]->command_tokens;
        while (*alias_command)
        {
            len += strlen(*alias_command) + 1;
            ++alias_command;
        }
        char *alias_value = malloc(len);
        if (!alias_value)
        {
            perror("allocation error");
            return;
        }

        alias_command = aliases[i]->command_tokens;
        strcpy(alias_value, aliases[i]->name);
        strcat(alias_value, " ");
        while (*alias_command)
        {
            strcat(alias_value, *alias_command);
            strcat(alias_value, " ");
            ++alias_command;
        }
        strcat(alias_value, "\n");
        fputs(alias_value, file);
        free(alias_value);
    }
    fclose(file);
}

// this function is used to read the any aliasess in from the '.aliases' file
int read_aliases(Alias **aliases)
{
    int aliases_len = 0;

    FILE *file = fopen(ALIASES_FILENAME, "r");
    if (file == NULL)
    {
        file_error(ALIASES_FILENAME);
        return 0;
    }
    char line[150];
    
    // this loop gets each line of the file and parses it into an alias
    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *input_string = malloc(sizeof(char) * 150);
        if(!input_string){
            perror("allocation error");
            return 0;
        }
        strcpy(input_string, line);
        char *token_maker = input_string;
        char **input_tokens = get_tokens(token_maker);
        char *name = *input_tokens;
        ++input_tokens;
        Alias *input_alias = create_alias(name, input_tokens);
        aliases_len += add_alias(aliases, input_alias, aliases_len);
    }
    fclose(file);
    return aliases_len;
}

// this function adds an alias to the aliases array
int add_alias(Alias **aliases, Alias *alias, int aliases_len)
{
    // if the alias array is full show an appropriate message
    if (aliases_len >= MAX_ALIASES)
    {
        aliases_full_err();
        return 0;
    }

    int i = 0;
    while (*(aliases + i))
        ++i;
    aliases[i] = alias;

    return 1;
}

// this function get an alias from the alias array and returns the command belonging
// to that alias
char **fetch_alias(char **tokens, char **alias_command)
{
    int alias_command_len = 0;
    while(alias_command[alias_command_len]) ++alias_command_len;
    int tokens_len = 0;
    while(tokens[tokens_len]) ++tokens_len;

    char **new_tokens = malloc((tokens_len+alias_command_len+1) * sizeof(char *));
    
    if (!tokens)
    {
        perror("allocation error");
        return NULL;
    }

    for(int i = 0; i < alias_command_len; i++){
        new_tokens[i] = alias_command[i];
    }
    for(int i = 0; i < tokens_len; i++){
        new_tokens[alias_command_len+i] = tokens[i];
    }

    new_tokens[alias_command_len+tokens_len] = NULL;

    return new_tokens;
}

// this function is called from the main to get the alias command
char **get_alias_command(Alias *alias, char **tokens)
{
    ++tokens;
    char **command = alias->command_tokens;
    return fetch_alias(tokens, command);
}

void print_aliases(Alias **aliases, int aliases_len)
{
    for (int i = 0; i < aliases_len; i++)
    {
        Alias *alias = aliases[i];
        printf("{ %s : \" ", alias->name);
        char **tokens = alias->command_tokens;
        while (*tokens != NULL)
        {
            printf("%s ", *tokens);
            ++tokens;
        }
        printf("\" }\n");
    }
    
}

// this function is used to remove an alias from the aliases array
Alias **remove_alias(Alias **aliases, char *name, int *aliases_len)
{
    for(int i = 0; i < *aliases_len; i++)
    {
        Alias *alias = aliases[i];
        if (!strcmp(alias->name, name))
        {
            // free(alias);
            for(int j = i; j < *aliases_len; j++)
            {
                if(aliases[j])
                {
                    aliases[j] = aliases[j+1];
                }
            }

            aliases[*aliases_len] = NULL;
            (*aliases_len)--;
            break;
        }
    }
    return aliases;
}

void update_alias(Alias **aliases, Alias *alias, char **command, int aliases_len){
    for(int i = 0; i < aliases_len; i++){
   
        if(!strcmp(aliases[i]->name, alias->name)){
            // Count the number of tokens in the new command
            int num_tokens = 0;
            while (command[num_tokens] != NULL) num_tokens++;

            // Allocate memory for the new command tokens
            aliases[i]->command_tokens = malloc((num_tokens + 1) * sizeof(char *));
            if (aliases[i]->command_tokens == NULL)
                allocation_error();
            

            // Copy each token
            for (int j = 0; j < num_tokens; j++) {
                aliases[i]->command_tokens[j] = strdup(command[j]);
                if (aliases[i]->command_tokens[j] == NULL) {
                    allocation_error();
                }
            }
            aliases[i]->command_tokens[num_tokens] = NULL; // Null-terminate the array
            break;
        }
    }

    alias_override_msg(alias->name);
}

void free_aliases(Alias **aliases, int aliases_len)
{
    for(int i = 0; i < aliases_len; i++)
    {
        free(aliases[i]); 
    }
    free(aliases);
}
