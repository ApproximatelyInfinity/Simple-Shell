#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main (int argc, char **argv)
{
	
    char *buff[30];
	char path[30];
	char *t;
	char CLine[30];
    char *Coman;
	int i;
	int j;
	int cntr = 0;
	char *p;
	char *CArg;
	int fds[2];
	int status;
	int pid;
	pid_t child2;
	pid_t child;
	
	
	
	p = getenv("PATH");   /*For grabbing the path data from the system*/
	
	t = strtok(p, ":");  /*Tokenize path data */
	
    while (t != NULL)	/* until pointer t is NULL, fill path wiht data pointed*/
    {					/*	  to by t */
	cntr++;			/*keeps track of command length for differant structures*/
        buff[i] = t;
	i++;
        t = strtok(NULL, ":");
    }

  while(1)						/* Loop until user commands exit or quit*/
  {
  	fprintf(stderr, "cjShell:>> ");		/*shell prompt*/
	fgets(CLine, 30, stdin);	/*grabs user determined command*/
	
	CLine[strlen(CLine) - 1] = '\0';  /* Puts NULL character at end of array to 
										prevent garbage values from leaking*/
	if(strcmp(CLine, "quit") == 0 || (strcmp(CLine, "exit") == 0)) /*user exits*/
	{
		exit(0);
	}else{
		Coman = strtok(CLine, " ");
		CArg = strtok(NULL, " ");
	
		pipe(fds);
		child = fork();
		child2 = fork();
		if(child == 0)	//sees if the child process is active or not
		{	 
			close(1);
			dup(fds[1]);
			close(fds[0]);
			
			for(i = 0; i < cntr; ++i)
			{
				strcpy(path, buff[i]);    /*copys data from buff into path */
				strcat(path, "/");			
				strcat(path, Coman);
				execl(path,Coman,CArg,0);    /*execute system call containing path data, user
												entered command, and any additional arguments*/
			}
			fprintf(stderr, "\nCommand NOT found!\n");
		}
		if(child2 == 0) //sees if another child process is active or not
		{
			close(0);
			dup(fds[0]);
			close(fds[1]);
			
			for(i = 0; i < cntr; ++i)
			{
				strcpy(path, buff[i]);    /*copys data from buff into path */
				strcat(path, "/");			
				strcat(path, Coman);
				execl(path,Coman,CArg,0);    /*execute system call containing path data, user
												entered command, and any additional arguments*/
			}
			
		}
		
		close(fds[0]);
		close(fds[1]);
		
		do 
		{
			pid = wait(&status);
			if(pid == child)
				child = 0;
			if(pid == child2)
				child2 = 0;
		
		}while(child != child2 && pid != -1);
		
		 
	}
	
  }
    
}










