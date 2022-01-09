/**
 *
 * @file
 *
 * Text-based UI functionality
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "history.h"
#include "util.h"
#include "ui.h"

int commandNum=0;
int commandNum2=0;
struct hist *arr;
int lim=0;
int size=0;
void hist_init(unsigned int limit)
{
    // TODO: set up history data structures, with 'limit' being the maximum
    // number of entries maintained.
    arr=calloc(limit,sizeof(struct hist));
    lim=limit;
}

void hist_destroy(void)
{
	int i;
	if(size>=lim){
		for(i=0;i<lim;i++){
			free(arr[i].command);
		}
	}
	else{
		for(i=0;i<size;i++){
			free(arr[i].command);
		}
	}
	free(arr);
}

void hist_add(const char *cmd)
{
	if(commandNum2==lim){
		commandNum2=0;
	}
	if(strcmp(cmd,"")!=0){
		arr[commandNum2].num=commandNum+1;
		if(size<lim){
			arr[commandNum2].command=malloc(4096);
		}
		strcpy(arr[commandNum2].command,cmd);
		commandNum++;
		commandNum2++;
		size++;
	}
	else{	
		decrement();
	}
}

void hist_print(void)
{
	int i=0;
	if(size<=lim){
		for(i=0;i<commandNum;i++){
			printf("%3d  ",arr[i].num);
			printf("%s\n",arr[i].command);
		}
	}
	else{
		for(i=commandNum2;i<lim;i++){
			printf("%3d  ",arr[i].num);
			printf("%s\n",arr[i].command);
		}
		for(i=0;i<commandNum2;i++){
			printf("%3d  ",arr[i].num);
			printf("%s\n",arr[i].command);
		}
	}
	fflush(stdout);
}

const char *hist_search_prefix(char *prefix)
{
    // TODO: Retrieves the most recent command starting with 'prefix', or NULL
    // if no match found.
    int i;
    if(size<=lim){
	for(i=commandNum2-1;i>=0;i--){
		if(strncmp(arr[i].command,prefix,strlen(prefix))==0){
			return arr[i].command;
		}
	}
	return NULL;
    }
    else{
	for(i=commandNum2-1;i>=0;i--){
		if(strncmp(arr[i].command,prefix,strlen(prefix))==0){
			return arr[i].command;
		}
	}
	for(i=lim-1;i>=commandNum2;i--){
		if(strncmp(arr[i].command,prefix,strlen(prefix))==0){
			return arr[i].command;
		}	
	}
    }
    return NULL;
}

const char *hist_search_cnum(int command_number)
{
    // TODO: Retrieves a particular command number. Return NULL if no match
    // found.
    int i;
    if(size<=lim){
	for(i=0;i<commandNum2;i++){
		if(arr[i].num==command_number){
			return arr[i].command;
		}
	}
	return NULL;
    }
    else{
	for(i=0;i<lim;i++){
		if(arr[i].num==command_number){
			return arr[i].command;
		}
	}
    }
    return NULL;
}

unsigned int hist_last_cnum(void)
{
    // TODO: Retrieve the most recent command number.
    return (commandNum-1);
}
