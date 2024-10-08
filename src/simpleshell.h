
#define BUFFER_SIZE 1024
#define INPUT_LIMIT 512
#define TOKEN_DELIM " \t\n|><&;"

#define HISTORY_SIZE 20
#define HISTORY_FILENAME ".history"

#define MAX_ALIASES 10
#define ALIASES_FILENAME ".aliases"

// STRUCT
typedef struct Alias
{
    char *name;
    char **command_tokens;
} Alias;

// MAIN.c FUNCTIONS
int run(char *user_in, char **history, int *history_len, int *history_index, Alias ***aliases, int *aliases_len);


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

void print_tokens(char **tokens);

void free_tokens(char **tokens);

void reset_env(char *starting_dir, char *starting_HOME, char *starting_PATH);


// History.c

char **check_history(char ***history, int *history_len, int *history_index, char **tokens, char *user_in);

char **create_history_array();

void print_history(char ** history, int history_len, int history_index);

void save_history(char **history, int history_len, int history_index);

int read_history(char **history);

int parseHistoryPosition(char *command);


// void add_to_history(char **history, int *history_index, int *history_len, char *user_in);

// aliases.c

void check_aliases(Alias ***aliases, char ***tokens, int *aliases_len);

Alias **create_alias_array();

Alias *alias_exists(Alias **aliases, char *name, int aliases_len);

Alias *create_alias(char *name, char **tokens);

void add_new_alias(Alias ***aliases, char **tokens, int *aliases_len);

int add_alias(Alias **aliases, Alias *alias, int aliases_len);

char **fetch_alias(char **tokens, char **alias_command);

char **get_alias_command(Alias *alias, char **tokens);

void print_aliases(Alias **aliases, int aliases_len);

void unalias(Alias ***aliases, char **tokens, int *aliases_len);

Alias **remove_alias(Alias **aliases, char *name, int *aliases_len);

void save_aliases(Alias **aliases, int aliases_len);

int read_aliases(Alias **aliases);

void update_alias(Alias **aliases, Alias *alias, char **command, int aliases_len);

void free_aliases(Alias **aliases, int aliases_len);

// ERRORS.c
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

void file_error(char *file_name);

void allocation_error();

//MESSAGES  
void alias_override_msg(char *name);

// TESTING
void testing_mode();

void test_tokens(char ** tokens);
