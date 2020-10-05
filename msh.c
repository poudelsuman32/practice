/*
  Name: Suman Raman Poudel
  ID:1001750938

  */


// The MIT License (MIT)
//
// Copyright (c) 2016, 2017, 2020 Trevor Bakker
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // The maximum command-line size

#define MAX_NUM_ARGUMENTS 5     // Mav shell only supports five arguments

int main()
{
  //intializing and declaring variable whichj are importants for latter parts
  char * cmd_str = (char*) malloc( MAX_COMMAND_SIZE );
  int pid_index=0;
  int pid_history[15];
  char*history[15];
  int i;
  int status;
  int show_pid[15];
  int count;
  int ind;
  int last;

  while( 1 )
  {
    // Print out the msh prompt
    printf ("msh> ");

    // Read the command from the commandline.  The
    // maximum command that will be read is MAX_COMMAND_SIZE
    // This while command will wait here until the user
    // inputs something since fgets returns NULL when there
    // is no input
    while( !fgets (cmd_str, MAX_COMMAND_SIZE, stdin) );

    /* Parse input */

    if(cmd_str[0]=='\n')
    {
      continue;
    }
    char *token[MAX_NUM_ARGUMENTS];

    int   token_count = 0;

    // Pointer to point to the token
    // parsed by strsep
    char *argument_ptr;

     strncpy(history[pid_index],cmd_str,255);
     pid_index++;
     if(pid_index>14) pid_index=0;

    char *working_str  = strdup( cmd_str );

    // we are going to move the working_str pointer so
    // keep track of its original value so we can deallocate
    // the correct amount at the end
    char *working_root = working_str;

    // Tokenize the input stringswith whitespace used as the delimiter
    while ( ( (argument_ptr = strsep(&working_str, WHITESPACE ) ) != NULL) &&
              (token_count<MAX_NUM_ARGUMENTS))
    {
      token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
      if( strlen( token[token_count] ) == 0 )
      {
        token[token_count] = NULL;
      }
        token_count++;
    }
       // ----Requirements 5--- Exiting or quitting

     if((strcmp(token[0],"exit")) || (strcmp(token[0],"quit")))
     {
       exit(0);
     }

   //---Requirements-10----changining the directory

     else if(strcmp(token[0],"cd")==0)
     {
       chdir(token[1]);
     }
     //---Requirements-12 ----printing the history
    else  if(strcmp(token[0],"history")==0)
     {

       history[pid_index++]=fork();
       if(pid_index<15)
       {
         pid_index++;
       }
       else
       {
         int index=pid_index;
         for(i=0;i<15;i++)
         {
           printf("%s\n",history[index]);
           if(index>14) index=0;
         }
       }

     }
     //---Printing the history--- as by the user given number
     else if(token[0][0]=='!')
     {
       pid_history[pid_index++]=fork();
       int story=atoi(&token[0][1]);
       if(story>15)
       {
         printf("Command not in history");
       }
       else
       {
         strncpy(working_str,pid_history[story],255);
         printf("%d\n",pid_history[story]);
       }

     }
     //--Requirements -11-----Printing PIDS

     else if(strcmp(token[0],"showpids")==0)
     {
       i=0;
       count=0;
       ind=0;
       while(show_pid[i]!=0)
       {
         count++;
       }

       if(count<15)
       {
         for(i=0;i<count;i++)
         {
           printf("Pids of [%d] is: %d \n",i,show_pid[i]);
         }
       }

       else
       {
         last=ind;

         for(i=0;i<15;i++)
         {
           printf("Pids of [%d] is : %d \n",i,show_pid[i]);
           last++;

           if(last>14)
           {
             last=0;
           }
         }

       }

     }
     //-----Requirements 2---Testing the user command correct or not
     else
     {
       pid_t child_pid=fork();
       if(child_pid==0)
       {
         int ret=execvp(token[0],&token[0]);
         if(ret==-1)
         {
           printf("Command not found\n");
         }

       }
       else
       {
         show_pid[ind++]=child_pid;

         if(ind>14)
         {
           ind=0;
         }

         wait(&status);
       }

     }

    free( working_root );

  }
  return 0;
}
