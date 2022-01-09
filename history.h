/**
 * @file
 *
 * Contains shell history data structures and retrieval functions.
 */

#ifndef _HISTORY_H_
#define _HISTORY_H_

/**
 *
 *Struct to hold command number and command
 *
 */
struct hist{
	int num;
	char *command;
};

/**
 *
 *Intitializes everything needed to gather history information
 *@param limit of limit size
 *
 */
void hist_init(unsigned int);

/**
 *
 *Destorys everything allocated by hist_init
 *
 */
void hist_destroy(void);

/**
 *
 *Creates a struct hist and then adds it to the array
 *@param cmd is a a command it will add to history
 *
 */
void hist_add(const char *);

/**
 *
 *Prints the history of the limit of commands
 *
 */
void hist_print(void);

/**
 *
 *Searches for the most recent command 
 *@param prefix is a prefix which will be searched for
 *
 */
const char *hist_search_prefix(char *);

/**
 *
 *Searches for a specific command number
 *@param command_number is the command number it will search for
 *
 */
const char *hist_search_cnum(int);

/**
 *
 *Searches for the most recent command number
 *
 */
unsigned int hist_last_cnum(void);

#endif
