#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "simpleshell.h"


Alias **create_alias_array()
{
    Alias **aliases = malloc(MAX_ALIASES * sizeof(Alias *));
    if (!aliases)
    {
        perror("<Allocation Error>");
    }
    return aliases;
}

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

Alias *create_alias(char *name, char **tokens)
{
    Alias *alias = malloc(sizeof(Alias));
    alias->name = name;
    alias->command_tokens = tokens;

    return alias;
}

void save_aliases(Alias **aliases, int aliases_len)
{
    FILE *file;
    file = fopen(ALIASES_FILENAME, "w");
    if (file == NULL)
    {
        file_error(ALIASES_FILENAME);
        return;
    }
    for (int i = 0; i < aliases_len; i++)
    {
        int len = strlen(aliases[i]->name);
        char **alias_command = aliases[i]->command_tokens;
        while (*alias_command)
        {
            len += strlen(*alias_command) + 1;
            ++alias_command;
        }
        char *alias_value = malloc(len);

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

    while (fgets(line, sizeof(line), file) != NULL)
    {
        char *input_string = malloc(sizeof(char) * 150);
        strcpy(input_string, line);
        char *terminator = input_string;
        while(*terminator != ' ')
            terminator++;
        terminator = '\0';

        char **input_tokens = get_tokens(input_string);
        char *name = *input_tokens;
        ++input_tokens;

        Alias *input_alias = create_alias(name, input_tokens);
        aliases_len += add_alias(aliases, input_alias, aliases_len);
    }
    return aliases_len;
}

int add_alias(Alias **aliases, Alias *alias, int aliases_len)
{
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

char **fetch_alias(char **tokens, char **alias_command)
{
    char **new_tokens = malloc(sizeof(tokens) + sizeof(alias_command));
    int i = 0;
    while (alias_command[i])
    {
        new_tokens[i] = alias_command[i];
        ++i;
    }
    int k = 0;
    if(tokens[k]){
        while (tokens[i])
        {
            new_tokens[i] = tokens[k];
            ++i;
            ++k;
        }
    }
    new_tokens[i] = NULL;
    return new_tokens;
}

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

Alias **remove_alias(Alias **aliases, char *name, int *aliases_len)
{
    // Alias **new_aliases_list = create_alias_array();
    // int i = 0;
    // while (*aliases)
    // {
    //     Alias *alias = *aliases;
    //     if (!strcmp(alias->name, name))
    //     {
    //         ++aliases;
    //     }else{
    //         new_aliases_list[i] = alias;
    //         i++;
    //         ++aliases;
    //     }
    // }
    // return new_aliases_list;
    for(int i = 0; i < *aliases_len; i++)
    {
        Alias *alias = aliases[i];
        if (!strcmp(alias->name, name))
        {
            for(i; i < *aliases_len; i++)
            {
                if(aliases[i])
                {
                    printf("here\n");
                    aliases[i] = aliases[i+1];
                }
            }
        }
    }
    return aliases;
}

