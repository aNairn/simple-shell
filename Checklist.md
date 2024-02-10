# Purpose

Use this file to breakdown tasks and track progess.

Please update this file with anything you think needs done we can discuss stuff further.

# Stage One

- [x] Display Prompt  -  Display a prompt to the screen while waiting on the users input
- [x] Read users input - Saving it in an appropriately named variable 
- [x] Parse the input checking for type and character limit
- [x] Split the input into 'tokens' separated by white space (512)
- [x] Allow continues reading until exit triggered through input "exit" and ctr-D

# Stage Two

- [x] Use fork() to create a child process
- [x] Prompt the parent process to wiat on the child to complete
- [x] Have the child process run the exec() function to run external commands  using the previously entered tokens.
- [x] catch errors appropriatly (moday 29th lecture)

# Stage Two Strech
- [] Write test file
- [x] Add final perrors
- [] Possible error file

# Stage Three
- [x] get the home and current directory and save them for use on shutdown
- [x] save the PATH at the start of the program
- [x] have the program start in the users home dir
- [x] use getenv() to get users HOME dir
- [x] change the current dirrectory of the program using chdir()
- [x] use getcwd() finction to check this is working
- [x] implement getpath() setpath() functions
- [x] handle inproper param entered to getpath() and setpath() note: these functions return int values dependent on success

# Stage Four
- [x] check for cd command
- [x] check for to many args
- [x] change to an existing directory
- [x] show error for if a dir does not exist