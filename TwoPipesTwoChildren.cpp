// file: TwoPipesTwoChildren.cpp
// author: Devon Hayworth
// date: 04/02/2017
// purpose: CS 3376
// description: This program executes "ls -ltr | grep 3376 | wc -l" by using
// 		two pipes and two children.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char** argv)
{
	int status;
	int childpid1;
	int childpid2;

	char* ls_args[] = {"ls", "-ltr", NULL};
	char* grep_args[] = {"grep", "3376", NULL};
	char* wc_args[] = {"wc", "-l", NULL};

	int pipe1[2]; // first pipe to send the output of the "grep" process
		      // to the "wc" process
	int pipe2[2]; // second pipe to send the output of the "ls" process
		      // to the "grep" process
	pipe(pipe1);
	pipe(pipe2);

	if((childpid1 = fork()) == -1)
	{
		perror("Error creating a child process");
		exit(1);
	}
	if(childpid1 == 0)
	{
		dup2(pipe1[1], 1); // replace the first child's stdout with
				   // the write end of the first pipe
		dup2(pipe2[0], 0); // replace the first child's stdin with
				   // the read end of the second pipe
		close(pipe1[0]);   // close all pipes
		close(pipe1[1]);
		close(pipe2[0]);
		close(pipe2[1]);
		execvp(*grep_args, grep_args);
		exit(0);
	}
	else
	{
		if((childpid2 = fork()) == -1)
		{
			perror("Error creating a child process");
			exit(1);
		}
		if(childpid2 == 0)
		{
			dup2(pipe2[1], 1); // replace the second child's
					   // stdout with the write end of
					   // the second pipe
			close(pipe1[0]);   //close all pipes
			close(pipe1[1]);
			close(pipe2[0]);
			close(pipe2[1]);
			execvp(*ls_args, ls_args);
			exit(0);
		}
		else
		{
			dup2(pipe1[0], 0); // replace the parent's stdin with 
					   // the read end of the first pipe
			close(pipe1[0]);   // close all pipes
			close(pipe1[1]);
			close(pipe2[0]);
			close(pipe2[1]);
			execvp(*wc_args, wc_args);
		}
	}
	return 0;
}
