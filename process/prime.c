//利用交叉分配的方法来进程多进程并发
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define LEFT 10
#define RIGHT 410
#define N 4

int main(void){
	int n = 0 ;
	pid_t pid;
	int i = 0;
	int prime = 0;
	int j = 0;
	for( n = 0 ; n < N ; n++){
		pid = fork();
		if(pid < 0 ){
			perror("fork()");
			break;
		}
		if(pid == 0)   /*chrild*/
		{
			for(prime = LEFT+n ;  prime <= RIGHT ; prime += N){
				for(j = 2 ; j < prime /2 ; j++){
					if(prime % j == 0){
						break;
					}
				}
				if(prime % j != 0){
					printf("[%d] : %d is prime\n",n , prime);
				}
			}
			exit(0); //一个子进程结束了，必须得进行推出操作
		}
	}
	
	//收尸操作
	for(i = 0 ; i <= n ; i++){
		wait(NULL);
	}
	exit(0);
}
