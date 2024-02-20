
// STRUCT
typedef struct Alias
{
    char *name;
    char **command_tokens;
} Alias;

// MAIN.c FUNCTIONS
int run(char *user_in, char **history, int *history_len, Alias ***aliases, int *aliases_len);


// SIMPLESHELL.c FUNCTIONS
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


// ERRORS
void to_few_args_err();

void input_too_long_error();

int input_is_valid(char * input);

void fork_error();

void to_many_args_err();

void not_valid_dir(char **tokens);

void cd_into_file_error(char ** tokens);

void empty_history_error();

void parsing_int_error();

void value_out_of_bounds_error();

void aliases_full_err();

void no_alias_found_err();


// TESTING
void testing_mode();

void test_tokens(char ** tokens);
