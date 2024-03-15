#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>

#include "simpleshell.h"


// this function gets the current working directory
char *get_cwd()
{
    char cwd[BUFFER_SIZE];
    char *cwd_p = cwd;
    getcwd(cwd, sizeof(cwd));
    return cwd_p;
}

// this function displays the user prompt
void display_prompt(char *cwd)
{
    printf("%s:$>", cwd);
}

// this method gets the users input
char *get_users_input()
{
    char buffer[BUFFER_SIZE];
    char *user_in;
    user_in = fgets(buffer, BUFFER_SIZE, stdin);
    return user_in;
}

// this function gets the tokens from the input string
char **get_tokens(char *input)
{
    int i = 0;
    int buff = 16;
    char **tokens = malloc(buff * sizeof(char *));
    char *token;

    if (!tokens)
    {
        perror("allocation error");
        free(tokens);
        return NULL;
    }

    token = strtok(input, TOKEN_DELIM);

    while (token != NULL)
    {
        tokens[i] = token;
        i++;
        if (i > buff)
        {
            buff *= 2;
            tokens = realloc(tokens, buff * sizeof(char *));
            if (!tokens)
            {
                perror("<Allocation Error>");
                free(tokens);
                return NULL;
            }
        }
        token = strtok(NULL, TOKEN_DELIM);
    }
    tokens[i] = NULL;
    return tokens;
}

// this function changes the current directory to the new one entered
void change_dir(char **tokens)
{
    // if there is no destination entered show an appropriate message
    if (*tokens == NULL)
    {
        to_few_args_err();
    }
    // if there are to many parameters entered show an appropriate message
    else if (*(tokens + 1) != NULL)
    {
        to_many_args_err();
    }
    else // if the destination is valid go to the stated destination
    {
        if (!strcmp(*tokens, "~") || !strcmp(*tokens, "HOME"))
        {
            chdir(getenv("HOME"));
        }
        else if (chdir(*tokens) != 0)
        {
            struct stat buf;
            stat(*tokens, &buf);
            FILE *f = fopen(*tokens, "r");
            if (f != NULL)
            {
                fclose(f);
                cd_into_file_error(tokens);
            }
            else
            {
                not_valid_dir(tokens);
            }
        }
    }
}

// this function changes the home directory to one chosen by the user
void change_home(char **tokens)
{
    // of no destination is entered show an appropriate message
    if (*tokens == NULL)
    {
        to_few_args_err();
    }
    // if to many parameters are entered then display an approriate message
    else if (*(tokens + 1) != NULL)
    {
        to_many_args_err();
    }
    else // if the new home is valid then set the enviroment home to the chosen destination
    {
        struct stat dir_stat;
        if (stat(*tokens, &dir_stat) != 0 || !S_ISDIR(dir_stat.st_mode))
        {
            perror("Directory does not exist");
        }
        else if (setenv("HOME", *tokens, 1) != 0)
        {
            perror("change_home error");
        }
        else
        {
            printf("NEW HOME SET : %s\n", *tokens);
        }
    }
}

// this function changes the system path
void change_path(char **tokens)
{
    // if there is no destination entered then show an appropriate message
    if (*tokens == NULL)
    {
        to_few_args_err();
    }
    // if there are to many parameters then show an appropriate message
    else if (*(tokens + 1) != NULL)
    {
        to_many_args_err();
    }
    else // if the path entered is valid then try to set the enviroment path to the new path
    {
        struct stat dir_stat;
        if (stat(*tokens, &dir_stat) != 0 || !S_ISDIR(dir_stat.st_mode))
        {
            perror("Directory does not exist");
        }
        else if (setenv("PATH", *tokens, 1) != 0)
        {
            perror("change_path error");
        }
        else
        {
            printf("NEW PATH SET : %s\n", *tokens);
        }
    }
}

// this function prints the enviroment home
void print_home()
{
    printf("%s\n", getenv("HOME"));
}

// this function prints the enviroment path
void print_path()
{
    printf("%s\n", getenv("PATH"));
}

// this function forks into a new process and checks that the entered command
// is a system command located in the enviroment path
void run_fork(char **tokens)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        fork_error();
    }
    else if (pid == 0)
    {
        execvp(tokens[0], tokens);
        perror(tokens[0]);
        exit(1);
    }
    else
    {
        wait(NULL);
    }
}

// this function resets the system enviroment 
void reset_env(char *starting_dir, char *starting_HOME, char *starting_PATH)
{
    chdir(starting_dir);
    setenv("HOME", starting_HOME, 1);
    setenv("PATH", starting_PATH, 1);
    printf("RESETING ENVIRO : \n\tHOME-%s \n\tPATH-%s\n", getenv("HOME"), getenv("PATH"));
    printf("Exiting...\n");
}



// this function prints the tokens (used for testing)
void print_tokens(char **tokens)
{
    while (*tokens != NULL)
    {
        printf("\"%s\"\n", *tokens);
        tokens++;
    }
}
