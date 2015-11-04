//		commands.c
//********************************************
#include "commands.h"
//********************************************
#define MAX_BUFF_SIZE 200
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to JobList, pointer VarList, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
int ExeCmd(LIST_ELEMENT **pJobsList, LIST_ELEMENT **pVarList, char* lineSize, char* cmdString)
{
	LIST_ELEMENT* pElem;
	LIST_ELEMENT* pElem_curr;
	char* cmd; 
	char* args[MAX_ARG];
	char *val;
	char pwd[MAX_LINE_SIZE];
	char* delimiters = " \t\n";  
	int pID = 0, i = 0, num_arg = 0;
	bool illegal_cmd = FALSE; // illegal command
	cmd = strtok(lineSize, delimiters);
	if (cmd == NULL)
		return 0; 
	args[0] = cmd;
	for (i=1; i<MAX_ARG; i++)
	{
		args[i] = strtok(NULL, delimiters); 
		if (args[i] != NULL) 
			num_arg++; 

	}
	/*************************************************/
	// Built in Commands
	/*************************************************/
	if (!strcmp(cmd, "cd") ) 
	{

		if (num_arg == 1) 
		{
			// here your code runs the child process
			pID = fork();
			if( pID == 0 ) {
				setpgrp(); // THIS IS THE COMMAND THAT EACH CHILD SHOULD
				// EXECUTE, IT CHANGES THE GROUP ID
				//    execv(...);// execute the needed process
				static char path[MAX_BUFF_SIZE + 1];
				if (!strcmp(args[1], "-")){

					if(chdir(args[1])==0){
						printf("heey --");
					}else{
						perror("cd");
					}
				}else{
					if(chdir(args[1])==0){
						printf("wazafak");
					}else{
						perror("cd");
					}


				}
				// Handle execv error if you got to this line
			} else if( pID>0) {
				// Do parent - shell work here
			} else {
				// handle the fork error here
				perror("cd");
			}
		}
		else 
		{
			printf("cd error: only one argument is required!\n");	

		}
	} 

	/*************************************************/
	else if (!strcmp(cmd, "pwd")) 
	{
		if (num_arg == 0) 
		{
				char* cwd;
				char buff[MAX_BUFF_SIZE + 1];
				cwd = getcwd( buff, MAX_BUFF_SIZE + 1 );
				if(cwd!=NULL)
				{
					printf("%s\n",buff);
				}
				else
				{
					perror("pwd");
				}

		}
		else 
		{
			printf("pwd error: no arguments required!\n");	
		}

	}

	/*************************************************/
	else if (!strcmp(cmd, "set"))
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "unset")) 
	{
		if (num_arg == 1) 
		{
			if (DelVar(pVarList, args[1]) != 0) 
				printf("smash error: > \"%s\" - variable not found\n", args[1]);
		} 
		else 
			illegal_cmd = TRUE;
	}

	/*************************************************/
	else if (!strcmp(cmd, "show")) 
	{
		if (num_arg <= 1) 
		{
			// Show all
			if (args[1] == NULL) 
				PrintVars(pVarList);
			// print one var only
			else 
			{
				val = GetVar(*pVarList, args[1]);
				if (val == NULL) 
					printf("smash error: > \"%s\" - variable not found\n", args[1]);
				else 
					printf("%s := %s\n", args[1], val);
			}
		} 
		else 
		{
			illegal_cmd = TRUE;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "jobs")) 
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{

	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{

	}
	/*************************************************/
	else if (!strcmp(cmd, "quit"))
	{

	} 
	/*************************************************/
	else // external command
	{
		ExeExternal(args, cmdString);
		return 0;
	}
	if (illegal_cmd == TRUE)
	{
		printf("smash error: > \"%s\"\n", cmdString);
		return 1;
	}
	return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char *args[MAX_ARG], char* cmdString)
{
	int pID;
	switch(pID = fork()) 
	{
		case -1: 
			// Add your code here (error)

			/* 
			   your code
			   */
		case 0 :
			// Child Process
			setpgrp();

			// Add your code here (execute an external command)

			/* 
			   your code
			   */

		default:
			// Add your code here

			/* 
			   your code
			   */
			break;
	}
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
	int pID;
	char ExtCmd[MAX_LINE_SIZE+2];
	char *args[MAX_ARG];
	if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
	{
		// Add your code here (execute a complicated command)

		/* 
		   your code
		   */
	} 
	return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to JobList
// Parameters: command string, pointer to JobList
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, LIST_ELEMENT** pJobsList)
{
	int pID, i;
	char* Command;
	char* delimiters = " \t\n";
	char *args[MAX_ARG];
	if (lineSize[strlen(lineSize)-2] == '&')
	{
		lineSize[strlen(lineSize)-2] = '\0';
		// Add your code here (execute a in the background)

		/* 
		   your code
		   */

	}
	return -1;
}

