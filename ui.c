/**
 * @file
 *
 * Text-based UI functionality.
 */
#include <stdio.h>
#include <readline/readline.h>
#include <locale.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <sys/types.h>

#include "history.h"
#include "logger.h"
#include "ui.h"

static int readline_init(void);
static char prompt_str[512] = {0};
static bool scripting=false;
static char *line=NULL;
static size_t line_sz=0;
static int command=1;
bool valid=true;

void modifybool(void){
	valid=false;
}
void reset(void){
	free(line);
}
void init_ui(void)
{
    LOGP("Initializing UI...\n");

    char *locale = setlocale(LC_ALL, "en_US.UTF-8");
    LOG("Setting locale: %s\n",
            (locale != NULL) ? locale : "could not set locale!");
    if (isatty(STDIN_FILENO)) {
        LOGP("stdin is a TTY; entering interactive mode\n");
    } else {
        LOGP("data piped in on stdin; entering script mode\n");
	scripting=true;
    }

    rl_startup_hook = readline_init;
}
void print(void){
	printf("\n%s",prompt_str);
}
void decrement(void){
	command--;
}

char *prompt_line(void) {
    char *login=getlogin();
    char hostname[HOST_NAME_MAX+1]={0};
    char cwd[256]={0};
    char temp[256]={0};
    struct passwd *home=getpwuid(getuid());
    getcwd(cwd,sizeof(cwd));
    gethostname(hostname,HOST_NAME_MAX+1);
    sprintf(temp,"%s",home->pw_dir);
    char *p=strstr(cwd,temp);
    if(p&&strncmp(cwd,home->pw_dir,strlen(home->pw_dir))==0){
	p=p+strlen(temp);
	if(valid==true){
    		sprintf(prompt_str,"[🙂]-[%d]-[%s@%s:~%s]$",command, login,hostname,p);
	}
	else{
    		sprintf(prompt_str,"[🤮]-[%d]-[%s@%s:~%s]$",command, login,hostname,p);
	}
	valid=true;
	command++;
    }
    else{
	if(valid==true){
    		sprintf(prompt_str,"[🙂]-[%d]-[%s@%s:%s]$",command,login,hostname,cwd);
	}
	else{
    		sprintf(prompt_str,"[🤮]-[%d]-[%s@%s:%s]$",command,login,hostname,cwd);	
	}
	command++;
    }
    return prompt_str;
}

char *read_command(void)
{
	
 
    if(scripting==true){
	size_t read_sz=getline(&line,&line_sz,stdin);
	if(read_sz==-1){
		perror("getline");
		return NULL;
	}
	line[read_sz-1] = '\0';
	return line;
    }
    else{
    	return readline(prompt_line());
    }
}

int readline_init(void)
{
    rl_bind_keyseq("\\e[A", key_up);
    rl_bind_keyseq("\\e[B", key_down);
    rl_variable_bind("show-all-if-ambiguous", "on");
    rl_variable_bind("colored-completion-prefix", "on");
    rl_attempted_completion_function = command_completion;
    return 0;
}

int key_up(int count, int key)
{
    /* Modify the command entry text: */
    rl_replace_line("User pressed 'up' key", 1);

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: reverse history search

    return 0;
}

int key_down(int count, int key)
{
    /* Modify the command entry text: */
    rl_replace_line("User pressed 'down' key", 1);

    /* Move the cursor to the end of the line: */
    rl_point = rl_end;

    // TODO: forward history search

    return 0;
}

char **command_completion(const char *text, int start, int end)
{
    /* Tell readline that if we don't find a suitable completion, it should fall
     * back on its built-in filename completion. */
    rl_attempted_completion_over = 0;

    return rl_completion_matches(text, command_generator);
}

/**
 * This function is called repeatedly by the readline library to build a list of
 * possible completions. It returns one match per function call. Once there are
 * no more completions available, it returns NULL.
 */
char *command_generator(const char *text, int state)
{
    // TODO: find potential matching completions for 'text.' If you need to
    // initialize any data structures, state will be set to '0' the first time
    // this function is called. You will likely need to maintain static/global
    // variables to track where you are in the search so that you don't start
    // over from the beginning.

    return NULL;
}
