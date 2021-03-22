#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define LINESIZ 1024
#define DATASIZ 1024

int main(int argc , char * argv[]){
	char *lineptr[LINESIZ];
	char buf[BUFSIZ];
	time_t t;
	struct tm *tmptr;
	size_t n ;
	int count = 0;
	char data_inflie[DATASIZ];
	if(argc != 2 ){
		fprintf(stderr,"传参不对\n");
		exit(1);
	}
	FILE* fp = fopen(argv[1],"a+"); //打开文件
	if(fp == NULL){
		perror("fopen()");
		exit(1);
	}
	
	//获取当前行数，读文件getline
	while(getline(lineptr,&n , fp) > 0){
		count++;
	}
	while(1){
		t = time(NULL);
		if(t == -1){
			perror("time()");
			exit(1);
		}
		tmptr = localtime(&t);
		if(tmptr == NULL){
			perror("localtime()");
			exit(1);
		}
		strftime(buf,BUFSIZ,"%F %T",tmptr);
		
		strcat(data_inflie , buf);
		strcat(data_inflie , "\n");
		fprintf(fp , "%-4d %s",++count,data_inflie);
		
		fflush(fp);
		sleep(1);
		
	}
	
	fclose(fp);
	exit(0);
}