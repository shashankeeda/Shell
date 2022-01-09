/**
 * @file
 *
 * Text-based UI functionality.
 */
#include <fcntl.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <ctype.h>

#include "history.h"
#include "logger.h"
#include "ui.h"
#include "util.h"

static char input_file[256]={0};
static char output_file[256]={0};
static int i;
static int output;
static int tokens;
static char *command;
static char jobs[12][2056]={0};
static int jobCount=0;
static char copy[2056]={0};
static int ind=0;
static int location[12]={0};
static int temp_arr[20]={0};
static bool flag=false;
/**
 *
 *Handles executing terminal commands
 *@param arr contains the positions of the pipes
 *@param args contains all the tokenized words
 *@param input_file is the input file if there is one
 *@param output_file is the output file if there is one
 *@param output determines what type of operation we should perform
 *
 */
void pipe_handler(int *arr, char *args[],char *input_file, char *output_file, int output){
	int j;
	int x=0;
	char *temp[]={"cat",input_file,NULL};
	bool tr=true;
	if(strlen(input_file)>0){
		i++;
		tr=false;
	}
	for(j=0;j<i;j++){
		int fd[2];
		if(pipe(fd)==-1){
			perror("pipe");
			return;
		}
			
		pid_t pid=fork();
		if(pid==-1){
			perror("fork");
			continue;
		}
		if(pid==0){
			close(fd[0]);
			dup2(fd[1],STDOUT_FILENO);
			int ret=0;
			if(tr==false&&j==0){
				ret=execvp(temp[0],temp);
			}
			else{
				ret=execvp(args[x],&args[x]);
			}
			if(ret==-1){
				perror("execvp");
			}
			close(fileno(stdin));
			close(fileno(stdout));
			close(fileno(stderr));
			close(fd[1]);
			return;	
		}
		else{
			close(fd[1]);
			dup2(fd[0],STDIN_FILENO);
			if(tr==false && j==0){
				x=0;	
			}
			else if(j!=i){
				x=arr[j]+1;
			}
			close(fd[0]);
		}	
	}
	if(output==0){
		execvp(args[x],&args[x]);
	}
	else if(output==1){
		int open_flags = O_RDWR | O_CREAT | O_TRUNC;
		int open_perms=0644;
		int f=open(output_file,open_flags,open_perms);
		dup2(f, STDOUT_FILENO);
		execvp(args[x],&args[x]);
		close(f);
	}
	else{
		int open_flags = O_RDWR | O_APPEND | O_CREAT;
		int open_perms=0644;
		int f=open(output_file,open_flags,open_perms);
		dup2(f, STDOUT_FILENO);
		execvp(args[x],&args[x]);
		close(f);
	}
	close(fileno(stdin));
	close(fileno(stdout));
	close(fileno(stderr));
	return;

}

/**
 *
 *Tokenizes everything into args
 *@param args is where we store the tokens
 *@param arr are the indexes of the pipe
 *@param next_tok gets the tokens one by one
 *
 */
void tokenize(char *args[], int *arr, char *next_tok){
	char *curr_tok;
	while((curr_tok=next_token(&next_tok, " \t\n\r"))!=NULL){
		args[tokens++]=curr_tok;
		if(strcmp(args[tokens-1],"|")==0){
			arr[i]=tokens-1;
			i++;
		}
		if(strchr(args[tokens-1],'#')!=NULL||strcmp(args[tokens-1],"|")==0){
			args[tokens-1]=NULL;
		}
		
		if(tokens>2&&args[tokens-2]!=NULL&&strcmp(args[tokens-2],"<")==0){
			args[tokens-2]=NULL;
			strcpy(input_file,args[tokens-1]);
			args[tokens-1]=NULL;
		}
		
		if(tokens>1 && args[tokens-2]!=NULL && (strcmp(args[tokens-2],">")==0 || strcmp(args[tokens-2],">>")==0)){	
				if(strcmp(args[tokens-2],">>")==0){
					output=2;
				}
				else{
					output=1;
				}
				args[tokens-2]=NULL;
				strcpy(output_file,args[tokens-1]);
				args[tokens-1]=NULL;
		}
    		
	}
}


/**
 *
 *Handles control c commands
 *@param signo is the handler parameter
 *
 */
void sigint_handler(int signo){
    signal(SIGINT, sigint_handler);
    if (isatty(STDIN_FILENO)) {
	    if(flag==false){
    		print();
	    }
	    else{
		printf("\n");
	    }
    }
    fflush(stdout);
}
/**
 *
 *Function handles any operation that starts with !
 *@param command is the command we will be tokenizing
 *@return char command
 *
 *
 */
char *not_operations(char *command){

	char buf[4096]={0};
	if(isdigit(command[1])!=0){
		bool truth=true;
		for(int w=0;w<strlen(command)-1;w++){
			if(isdigit(command[w+1])!=0){
				buf[w]=command[w+1];
			}
			else{
				truth=false;
				break;
			}
		}
		if(truth==true){	
			int t=atoi(buf);
			if(hist_search_cnum(t)!=NULL){
				strcpy(command,hist_search_cnum(t));
			}
		}
		hist_add(command);
	}
	else{	
		for(int w=0;w<strlen(command)-1;w++){
			buf[w]=command[w+1];
		}
		if(hist_search_prefix(buf)!=NULL){
			strcpy(command,hist_search_prefix(buf));
		}
		hist_add(command);
	}
	return command;
}

/**
 *
 *Handles most built in functions
 *@param command is the command it is looking at
 *@return int value depending on outcome
 *
 */
int built_ins(char *command){
	
	if(strcmp(command,"exit")==0){
		hist_destroy();
		reset();
		return 2;
	}

	if(strcmp(command,"!!")==0){
		hist_add(hist_search_cnum(hist_last_cnum()+1));
		strcpy(command,hist_search_cnum(hist_last_cnum()));
	}
	else if(strncmp(command,"!",1)==0 && strlen(command)>1){
		strcpy(command,not_operations(command));
	}
	if(strcmp(command,"history")==0){
		hist_print();
		return 1;
	}
	if(strcmp(command,"jobs")==0){
		for(int w=0; w<jobCount; w++){
			if(temp_arr[w]!=1){
				printf("%s\n",jobs[w]);
				temp_arr[w]=1;
			}
		}
		ind=0;
		return 1;
	}
	return 0;
}

/**
 *
 *Figures out the current running processes
 *@param signo is something we can ignore
 *
 *
 */
void sig_handler(int signo){
	int status;
	pid_t t=waitpid(-1,&status,WNOHANG);
	if(t!=0){
		int c=0;
		while(location[c]!=t){
			if(c>=9){
				break;
			}
			c++;
		}
		temp_arr[c]=1;
	}
}
/**
 *
 *THe main runs everything as well as the commands
 *
 */
int main(void)
{
    init_ui();
    hist_init(100);

    output=0;
    signal(SIGINT,sigint_handler);
    signal(SIGCHLD,sig_handler);
    while (true) {
	bool background=true;
        command = read_command();
        if (command == NULL) {
	    hist_destroy();
            break;
        }
	char *args[4096]={0};
	tokens=0;
	if(strncmp(command,"!",1)!=0||(strcmp(command,"!")==0 && strlen(command)==1)){
		hist_add(command);
		
	}
	int type=built_ins(command);
	if(type!=0){
		if(type==2){
			return EXIT_SUCCESS;
		}
		else{
			continue;
		}
	}
	char *next_tok=command;
	int arr[4096]={0};
	i=0;


	strcpy(copy,next_tok);

	tokenize(args,arr,next_tok);
	
	if(tokens>1 && args[tokens-1]!=NULL&&strcmp(args[tokens-1],"&")==0){
		background=false;
		strcpy(jobs[jobCount],copy);
		jobCount++;
		args[tokens-1]=NULL;
	}
	memset(copy,0,256);
	
	if(strncmp(command,"cd",2)==0){
		if(args[1]!=NULL){
			chdir(args[1]);
			continue;
		}
		else{
    			struct passwd *home=getpwuid(getuid());
			chdir(home->pw_dir);
			continue;
		}
	}
	args[tokens]=NULL;
	pid_t child=fork();
	if(child==-1){
		perror("continue");
		continue;
	}
	else if(child==0){
		pipe_handler(arr,args,input_file,output_file,output);
		return EXIT_FAILURE;
	}
	else{
		int status;
		if(background==false){
			location[ind]=child;
			ind++;
			waitpid(-1,&status,WNOHANG);
		}
		else{
			flag=true;
			waitpid(child,&status,0);
			flag=false;
		}
		if(status!=0&&strcmp(command,"")!=0){
			modifybool();
		}
		memset(input_file,0,256);
		memset(output_file,0,256);
		output=0;
		continue;
	}
    }
    reset();
    return 0;
}
