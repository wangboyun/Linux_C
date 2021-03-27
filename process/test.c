#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


int main(int argc , char * argv[]){
	
	pid_t pid;
	pid = fork();
	if(pid < 0){
		perror("fork()");
		exit(1);
	}
	else if(pid == 0){
		
		execvp(argv[0] , argv);
		perror("execl()");
		exit(1);
	}
	else	
		wait(NULL);
	
	exit(0);
}