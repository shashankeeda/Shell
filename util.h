/**
 * @file
 * 
 * Text-basedd UI functionality. These functions are primarily concerned with
 * interacting with the readline library
 */

#ifndef _UTIL_H_
#define _UTIL_H_

/**
 *
 *Funtion that tokenizes the string and returns it back
 *@param char **str_ptr is the string that we want to tokenize
 *@param const char *delim is the string containing the delimeters we want to tokenize by
 *@return char of tokens
 *
 *
 */
char *next_token(char **str_ptr, const char *delim);

#endif
