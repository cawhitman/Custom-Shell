/* execute.c - code used by small shell to execute commands */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>

typedef int bool;
#define true 1
#define false 0

int execute(char *argv[])
/*
 * purpose: run a program passing it arguments
 * returns: status returned via wait, or -1 on error
 *  errors: -1 on fork() or wait() errors
 */
{
  bool bgflag = false; 
  int	pid ;
  int	child_info = -1;
  int i=0;
  while(argv[i] != NULL)
  {
    //loop through array, if & move process to background
    if(strcmp(argv[i], "&") == 0)
    {
      bgflag = true;
      argv[i] = '\0';
      break;
    }
    i++;
  } 
  if ( argv[0] == NULL )		/* nothing succeeds	*/
    return 0;

  if ( (pid = fork())  == -1 )
    perror("fork");
  else if ( pid == 0 ){
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
  
    execvp(argv[0], argv);
    perror("cannot execute command");
    exit(1);
  } 
  else {
    if (!bgflag)
    {
      if ( waitpid(pid,&child_info,WCONTINUED) == -1 )
        perror("wait");  
    }
    else if (waitpid(pid,&child_info,WCONTINUED)){
      printf("Completed Process %d\n", pid);
      kill(pid,SIGKILL);
      }
      
  }
  return child_info;
}

