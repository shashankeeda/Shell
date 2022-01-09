/**
 * @file
 *
 * Text-based UI functionality. These functions are primarily concerned with
 * interacting with the readline library.
 */

#ifndef _UI_H_
#define _UI_H_

/**
 *
 *Completes the final documentation check
 *@param const char *text will take in a string
 *@param int start takes in a starting position
 *@param int end takes in a ending position
 *@return char a char command for the documentation.
 *
 */
char **command_completion(const char *text, int start, int end);
char *command_generator(const char *text, int state);

/**
 *
 *Initializes everything needed to run the shell
 *
 *
 */
void init_ui(void);

/**
 *
 *Goes through recent typed searches through up key
 *@param int count takes in the number of types key pressed
 *@param int key takes in a int representing the key
 *@return int value representing which search.
 *
 */
int key_up(int count, int key);

/**
 *
 *Goes through recent typed searches through up key
 *@param int count takes in the number of types key pressed
 *@param int key takes in a int representing the key
 *@return int value representing which search.
 *
 */
int key_down(int count, int key);

/**
 *
 *Returns a string for user to enter input
 *
 */
char *prompt_line(void);

/**
 *
 *Prints out the prompt and returns a malloced string of user input
 *
 *
 */
char *read_command(void);

/**
 *
 *Changes emoji face if user entered a bad command
 *
 */
void modifybool(void);

/**
 *
 *Re-prints the promt if cntrl c entered
 *
 */
void print(void);

/**
 *
 *Frees the memory of the prompt string
 *
 */
void reset(void);

/**
 *
 *Makes sure command doesnt increment if user clicks empty string
 *
 */
void decrement(void);

#endif
