
// functions

void display_prompt();

char * get_users_input();

char ** get_tokens(char * input);


// error handling

void input_too_long_error();

int input_is_valid(char * token);

void fork_error();


// test functions

void test_tokens(char ** tokens);