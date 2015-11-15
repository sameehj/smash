// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers

/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include "commands.h"
extern int GPid;
extern int Last_Bg_Pid;
extern int Susp_Bg_Pid;
extern int susp;
extern LIST_ELEMENT* JobsList;
extern char* L_Fg_Cmd;
extern int globalId;
//extern int jobs_start_time[MAX_JOBS];
//extern char lastPwd[MAX_LINE_SIZE+1];

void signal_handler(int signum) {

	switch (signum) {
	case SIGTSTP:
		;
		
			if (GPid != -1) {
				if(kill(GPid,SIGTSTP)==-1){
							perror(NULL);												
							free(L_Fg_Cmd);
							DelList(&JobsList);
							exit(-1);
				} else {
					printf("smash > signal SIGTSTP was sent to pid %d\n",GPid);
					InsertElem(&JobsList,L_Fg_Cmd, globalId ,GPid ,1);
					GPid = -1;
					L_Fg_Cmd[0] = '\0';

				}
			
			} 
		
		break;
		
		
		
		
	case SIGINT:
		;
			if (GPid != -1) {
				if(kill(GPid,SIGKILL)==-1){
					
							perror(NULL);												
							free(L_Fg_Cmd);
							DelList(&JobsList);
							exit(-1);
				} else {
					printf("smash > signal SIGKILL was sent to pid %d\n",GPid);
					GPid = -1;
					L_Fg_Cmd[0] = '\0';

				}
			}
		break;
		
		
		
		
	case SIGCHLD:
		;
		int pid;
		while ( (pid = waitpid(-1, NULL, WNOHANG)) > 0) {
        DelPID(&JobsList, pid);
		}	
		break;
	}
}