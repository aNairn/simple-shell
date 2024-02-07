
// functions

char * get_cwd();

void display_prompt(char * cwd);

char * get_users_input();

char ** get_tokens(char * input);

void change_dir(char ** tokens);

void change_home(char ** tokens);

void change_path(char ** tokens);

// error handling

void input_too_long_error();

int input_is_valid(char * token);

void fork_error();

void to_many_args_err();

// test functions

void test_tokens(char ** tokens);