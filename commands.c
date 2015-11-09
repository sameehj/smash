//		commands.c
//********************************************
#include "commands.h"
//********************************************
#define MAX_BUFF_SIZE 10
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to JobList, pointer VarList, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************

int jobs_start_time[MAX_JOBS]={0};
int globalId=1;

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

		if (num_arg == 1) {
			getcwd(pwd, MAX_LINE_SIZE);
			if (pwd == NULL){
				exit(-1);
			}

			else {

				pwd[MAX_LINE_SIZE-1]='\0';

				if(strcmp(args[1],"-")==0){
					if(lastPwd == NULL){
						printf("cd: Error no previous path was found\n");
					}
					else {int res = chdir(lastPwd);
						if(res == -1){
							printf("smash error: > \"%s\" - path not found\n", lastPwd );      
						}
						else {
							printf("%s\n",lastPwd);
							strcpy(lastPwd , pwd);
						}
					}


				}

				else{
					int res = chdir(args[1]);
					if(res == -1){
						printf("smash error: > \"%s\" - path not found\n", args[1] );    
					} 
					else {
						strcpy(lastPwd , pwd);
					}

				}


			}
		}
		else  { illegal_cmd = TRUE;}
	} 




	/*************************************************/


	else if (!strcmp(cmd, "pwd")) 
	{
		if (num_arg == 0) 
		{
			char* cwd;
			char buff[MAX_LINE_SIZE + 1];
			cwd = getcwd( buff, MAX_LINE_SIZE + 1 );
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
			illegal_cmd = TRUE;
		}

	}

	/*************************************************/
	else if (!strcmp(cmd, "mkdir"))
	{

		if (num_arg == 1) 
		{
			char* cwd;
			cwd = getcwd( pwd, MAX_LINE_SIZE + 1 );
			if(cwd!=NULL)
			{
				char* path=malloc((strlen(pwd)+strlen(args[1]) + 1)*sizeof(char));
				if(path){
					strcpy(path,pwd);
					strcat(path,"/");
					strcat(path,args[1]);
					int status=mkdir(path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);	
					if(status==-1){
						struct stat status;

						if (stat(path, &status) == 0 && S_ISDIR(status.st_mode))
						{
							printf("smash error: > %s - directory already exists\n", args[1] );
						}else {
							printf("smash error: > %s - cannot create directory\n", args[1] );
						}	
					}

				}else{
					printf("mkdir: malloc error\n");
				}
			}
			else
			{
				perror("pwd");
			}

		}
		else 
		{
			illegal_cmd = TRUE;
		}
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
		if(num_arg==0){
			int size = 0;
			LIST_ELEMENT* jobs = *pJobsList;
			time_t timenow = time(NULL);
			LIST_ELEMENT* jobsArray[MAX_JOBS+1];
			while (jobs){
				size++;
				jobsArray[size]=jobs;
				jobs = jobs->pNext ;
			}
			jobs = *pJobsList;
			int i;
			for( i=1 ; i<=size ; i++){
				printf("[%d] %s : %d %d secs", i, jobsArray[size+1-i]->VarValue,jobsArray[size+1-i]->pID, (int)(timenow - jobs_start_time[i]));
				if (jobs->suspended == 1) {
					printf(" (Stopped)");
				}
				printf("\n");
				jobs = jobs->pNext ;
			}
		}


		else { illegal_cmd = TRUE;
		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "showpid")) 
	{
		if (num_arg == 0) 
		{
			printf("smash pid is %d\n",(int)getpid());
		}
		else{
			illegal_cmd = TRUE;

		}
	}
	/*************************************************/
	else if (!strcmp(cmd, "fg")) 
	{

	} 
	/*************************************************/
	else if (!strcmp(cmd, "bg")) 
	{
		int pID;
		switch(pID = fork()) 
		{
			case -1: 
				perror(NULL);
			case 0 :
				// Child Process
				setpgrp();	
				execvp(args[0],args);
			default:	
				waitpid(pID,NULL,WUNTRACED);
				break;
		} 

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
	switch(pID = fork()) //TODO check if something needs to be added with bg commands
	{
		case -1: 
			perror(NULL);
		case 0 :
			// Child Process
			setpgrp();	
			execvp(args[0],args);
		default:	
			waitpid(pID,NULL,WUNTRACED);
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
		if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
		{
			Command= lineSize;
			args[0] = "/bin/csh";
			args[1] = "-f";
			args[2] = "-c";
			args[3] = lineSize;
			args[4] = NULL;
		} else {
			args[0]= strtok(lineSize, delimiters);
			if (!args[0]){
				return 0;
			}
			for (i=1; i<MAX_ARG; i++)
			{
				args[i] = strtok(NULL, delimiters); 
			}
			Command=args[0];
		}

		printf("%s\n",Command);
		switch(pID = fork()) 
		{
			case -1: 
				perror(NULL);												
				DelList(pJobsList);
				exit(-1);
			case 0 :
				// Child Process
				setpgrp();
				if (execvp(args[0], args) == -1) {
					perror(NULL);
					exit(-1);
				}
				exit(-1);
			default:
				InsertElem(pJobsList,Command,globalId,pID,0);
				jobs_start_time[globalId]=time(NULL);
				globalId++;
				LIST_ELEMENT * jobs = *pJobsList;
				while (jobs){
				printf("%s\n",jobs->VarValue);
				jobs = jobs->pNext;
				}
				return 0;
		}
	}
	return -1;
}

