// C code to find the exit status of child process 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/wait.h> 

// Driver code 
int main(void) 
{ 
	pid_t pid = fork(); 
	
	if ( pid == 0 ) 
	{ 
		/* The pathname of the file passed to execl() 
			is not defined */
		execl("/bin/sh", "bin/sh", "-c", "./nopath", "NULL"); 
		printf("bbbbbbb\n");
		exit(0); //end of child , avoid print aaaaaa twice
	} 
	
	printf("aaaaaaaa\n");
	return 0; 
} 

