
// structs

typedef struct Alias
{
    char *name;
    char **command_tokens;
} Alias;

// functions

char *get_cwd();

void display_prompt(char *cwd);

char *get_users_input();

char **get_tokens(char *input);

void change_dir(char **tokens);

void change_home(char **tokens);

void change_path(char **tokens);

void print_home();

void print_path();

void run_fork(char **tokens);

void reset_env(char *starting_dir, char *starting_HOME, char *starting_PATH);

int parseCommand(char *command);

char **create_history_array();

void print_history(char **history, int history_len);

void print_tokens(char **tokens);

Alias **create_alias_array();

Alias *alias_exists(Alias **aliases, char *name);

Alias *create_alias(char *name, char **tokens);

int add_alias(Alias **aliases, Alias *alias, int aliases_len);

char **fetch_alias(char **tokens, char **alias_command);

char **get_alias_command(Alias *alias, char **tokens);

void print_aliases(Alias **aliases);

Alias **remove_alias(Alias **aliases, char *name);
