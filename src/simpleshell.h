
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

int parseCommand(char * command);

void print_history(char ** history, int history_len);

// test functions

void test_tokens(char ** tokens);
