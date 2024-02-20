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
- [x] Write test file
- [x] Add final perrors
- [x] Possible error file


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

# Stage Four - FIX
- [x] cd into a file should show an error saying its a file
- [x] getpath setpath gethome sethome
- [x] cd no arguments 
- [x] not enough arguments for change path and change home# Stage Five


# Stage Five
- [x] Create a list of Strings to hold the commands entered during runtime
- [x] Each time a command is ran add it to the History array
- [x] Check for '!' being the first character
- [x] Invoke command from history
        - [x] !! ==> print last command
        - [x] !1 ==> print first command
        - [x] !-1 ==> print first command 
- [x] If the number is greater or lower than the length of the History array throw error
- [x] Treat 0 of the array as 1 and run the command at the index entered 
        - [x] '!-1' ==> run the 0th command
        - [x] '!-4' ==> run the 3rd command
- [x] Pick a method of handling a full array
        - circular array?
- [x] Print History if the form of \<No> \<command>
- [x] If command found in history it should be ran like any other command but there should be no history check 

# Stage Six
- [ ] Create a new file to save the history to
        - might need to save positions if doing circular array.
- [ ] Save a history command per line of the txt file to make it easier to read back in
- [ ] Read history from the file
- [ ] Save history to the history array with all the corrct pointers
- [ ] Make sure the history stays in the same order between instances

# Stage Seven
