// file: DynPipe.cpp
// author: Devon Hayworth
// date: 04/02/2017
// purpose: CS 3376
// description: This program executes a pipe command dynamically.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main (int argc, char** argv)
{
	int status;
	int childpid;
	int i;
	int pipe1[2];
	int pipe2[2];
	bool switchPipes;
	char* cmd_args[3];

	if((argc >= 3) && (argc <= 6)) // checks if there are enough arguments
				       // on the command line to execute a
				       // pipe command
	{
		cmd_args[2] = NULL;
		pipe(pipe1);
		pipe(pipe2);
		dup2(pipe1[0], 0); // replace the parent's stdin with the read
				   // end of the first pipe
		switchPipes = false;
		for(i = argc - 2; i > 0; i--)
		{
			cmd_args[0] = strtok(argv[i], " \0"); // takes the
			cmd_args[1] = strtok(NULL, "\0");     // argument passed
							      // on the command
							      // line and puts
							      // it in an array
							      // that can be
							      // used by execvp
			if((childpid = fork()) == -1)
			{	
				perror("Error creating a child process");
				exit(1);
			}
			if(childpid == 0)
			{
				if(switchPipes)
				{
					dup2(pipe1[0], 0); // replace the
							   // child's stdin with
							   // the read end of
							   // the first pipe
					dup2(pipe2[1], 1); // replace the
							   // child's stdout
							   // with the write end
							   // of the second pipe
				}
				else
				{
					dup2(pipe1[1], 1); // replace the
							   // child's stdout
							   // with the write end
							   // of the first pipe
					dup2(pipe2[0], 0); // replace the
							   // child's stdin with
							   // the read end of
							   // the second pipe
				}
				close(pipe1[0]); 	   // close all pipes
				close(pipe1[1]);
				close(pipe2[0]);
				close(pipe2[1]);
				execvp(*cmd_args, cmd_args);
				exit(0);
			}	
			else
			{
				if(switchPipes)
				{
					close(pipe2[0]);     // close the second
					close(pipe2[1]);     // pipe
					pipe(pipe2);         // creates a new
							     // second pipe
					switchPipes = false; // switches which
							     // pipe is used to
							     // replace stdin
							     // and stdout in
							     // the next child
				}
				else
				{
					close(pipe1[0]);     // close the first
					close(pipe1[1]);     // pipe
					pipe(pipe1);         // creates a new
							     // first pipe
					switchPipes = true;  // switches which
							     // pipe is used to
							     // replace stdin
							     // and stdout in
							     // the next child
				}
			}
		}
		cmd_args[0] = strtok(argv[argc - 1], " \0"); // takes the last
		cmd_args[1] = strtok(NULL, "\0"); 	     // argument passed
							     // on the command
							     // line and puts it
							     // in an array that
							     // can be used by
							     // execvp
		execvp(*cmd_args, cmd_args);
	}
	else
	{
		perror("Invalid number of arguments");
		exit(1);
	}
	return 0;
}
