#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "list.h"
#include "vars.h"
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_JOBS 10
pNode stack_head;
typedef enum { FALSE , TRUE } bool;
int ExeComp(char* lineSize, LIST_ELEMENT** pJobsList);
int BgCmd(char* lineSize, LIST_ELEMENT** pJobsList);
int ExeCmd(LIST_ELEMENT **pJobsList, LIST_ELEMENT **pVarList, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
int GPid;// PID (global)
int Last_Bg_Pid;
time_t ttime;
int Susp_Bg_Pid;
char* L_Fg_Cmd;
int susp; //is the process suspended: 0- no, 1- yes
char lastPwd[MAX_LINE_SIZE+1];
#endif