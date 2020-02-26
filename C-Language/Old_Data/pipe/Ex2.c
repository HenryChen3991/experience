/*
# Filename: pipe1.c
#         Desc: an example of pipe communication application
#       Author: Licaibiao 
#      Version:  
#   LastChange: 2017-01-09  
#      History: 
 
=============================================================================*/
#include <sys/wait.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc, char *argv[])
{    
	pid_t pid;
	int fd[2];
	int read_count = 0;
	int i;
	char read_buffer[10] = {0};
	char write_buffer[10] = {0};
 
	/*create pipe*/
	if (pipe(fd) < 0)
	{
		printf("Create pipe failed\n");
		return -1;
	}
 
	/*create process*/
	if ((pid = fork()) < 0)
	{
		printf("Fork failed\n");
		return -1;
	}
 
	/* child */
	if (pid == 0)
	{
		printf("[child]Close read endpoint...\n");
		/* close read */
		close(fd[0]);   		
		for(i=0;i<10;i++)
		{
			write_buffer[i]=i;	
		}
		write(fd[1],write_buffer,i);
 
	}
	 /*father*/
	else
	{
		printf("[parent]Close write endpoint...\n");
		/* close write */
		close(fd[1]);   
		read_count = read(fd[0], read_buffer, 10);
		printf("father process read data\n");
		for(i=0; i<read_count; i++)
		{
			printf("read_buffer[%d] = %d\n",i,read_buffer[i]);
		}
	}
}
