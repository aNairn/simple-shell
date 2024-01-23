#include <stdio.h>
#include <string.h>
#include "simpleshell.h"

// functions

// update this to get user details? or CWD?
void display_prompt(){
    printf("simple shell:$>");
}

// error handling

void input_too_long_error(){
    printf("Input exceeds limit\n");
}
