# Project 2: Command Line Shell
This project is basically building a replica of the BASH shell. In order to build this project I ended creating multiple files that all work together. The main file is the shell.c file which will take in the command inputs as well as execute them. The shell.c file calls read_command which then calls prompt_line which will help print out the prompt as well as take in user input. We have a init_ui which which helps initialize everything as well as a hist_init which allocates memory for history data structures and will add commands as we input them. Basically my shell helps run simple commands such as echo and ls, and also uses pipes and I/O redirection to run more complex commands. In my shell.c file, the pipe_handler fucntion is the most important function which will basically run all the commands inputted unless its a built in command that doesn't need to be executed, the method sees whether there is pipes, I/O redirection, and if there is a file the data should be sent to, depending on all this it will execute. The tokenize function helps create tokens through the help of the next_token function in the util.c file and stores all the items in the args array. We also have sigint handler that will print out a new prompt every time control c is clicked allowing the user to type in a new command. We have a not_operations functions that will perform tasks if it detects a ! operator and then a built_ins function that will execute all the built in functions except cd since that needs to be execvpd. After everything is tokenized, it is executed by the child function while the parent waits. In the ui.c file the main two functions we are using are the prompt_line function as well as the read_command function which call prompt_line to print the prompt and then take in our input which will be run by shell.c. We also have mini helper functions like decrement, print, reset, and modifybool which help run things properly in the shell. We also have a key_up and key_down function which scrolls through the most recent history and a command_generator which helps with autocomplete. In the history.c function while we already have hist_init which initializes everything for us, we have a hist_destroy function which will free all the memory, a hist_add which will add the item we input into history, a hist_print which will print the 100 most recent commands, ahist_search_prefix which searches for the most recent command with that prefix, a hist_search_cnum which searches for a specific command number and return the command and a hist_last_cnum which returns the most recent command.
