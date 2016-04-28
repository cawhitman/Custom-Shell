/**  smsh1.c  small-shell version 1
 **		first really useful version after prompting shell
 **		this one parses the command line into strings
 **		uses fork, exec, wait, and ignores signals
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include "smsh.h"

#define	DFL_PROMPT	"> "
//Prototypes
void exitShell(int exitNum);
void goHome();

int main()
{
  char	*cmdline, *prompt, **arglist;
  int	result;
  void	setup();
  char cwd[250];
  getcwd(cwd, sizeof(cwd));
  prompt = strcat(cwd, "|(╯°□°)╯︵ ┻━┻| ");
  setup();

  while ( (cmdline = next_cmd(prompt, stdin)) != NULL ){
    if ( (arglist = splitline(cmdline)) != NULL  ){
      //Handle exit [num]
      if(strcmp(arglist[0],"exit") == 0)
      {
        if(arglist[1])
          {
            exitShell(atoi(arglist[1]));
          } 
       else
        {
         exit(0);
        }
      }
      //Handle chdir
      else if(strcmp(arglist[0],"cd") == 0)
      {
        if(arglist[1])
        {
          chdir(arglist[1]);
          getcwd(cwd, sizeof(cwd));
          prompt = strcat(cwd,"|(╯°□°)╯︵ ┻━┻| ");
        }
        else
        {
          goHome();
          getcwd(cwd, sizeof(cwd));
          prompt = strcat(cwd,"|(╯°□°)╯︵ ┻━┻| ");
        }
        
      }
      else 
      {
       result = execute(arglist);
      }
      freelist(arglist);
    }
    free(cmdline);
  }
  return 0;
}

void setup()
/*
 * purpose: initialize shell
 * returns: nothing. calls fatal() if trouble
 */
{
  signal(SIGINT,  SIG_IGN);
  signal(SIGQUIT, SIG_IGN);
}

void fatal(char *s1, char *s2, int n)
{
  fprintf(stderr,"Error: %s,%s\n", s1, s2);
  exit(n);
}

void exitShell(int exitNum)
{
  printf("Exited on: %d \n", exitNum);
  exit(exitNum);
}


void goHome()
{
  chdir(getenv("HOME"));
}