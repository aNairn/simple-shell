
// functions

char * get_cwd();

void display_prompt(char * cwd);

char * get_users_input();

char ** get_tokens(char * input);

void change_dir(char ** tokens);

void change_home(char ** tokens);

void change_path(char ** tokens);

void print_home();

void print_path();

void run_fork(char ** tokens);

void reset_env(char * starting_dir, char * starting_HOME, char * starting_PATH);

// error handling

void to_few_args_err();

void input_too_long_error();

int input_is_valid(char * token);

void fork_error();

void to_many_args_err();

void not_valid_dir(char ** tokens);

void cd_into_file_error(char ** tokens);

// test functions

void test_tokens(char ** tokens);
