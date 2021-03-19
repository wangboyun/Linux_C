#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int sumchar(int n);
int sum_file(int fd);

int main(void) {
	//统计第n行字符数
	int offset_11 = sumchar(11);
	int offset_10 = sumchar(10);
	int fd1,fd2;
	char buf[1];
	int read_ret, write_ret;
	int filechar ;
	//打开同一文件
	fd1 = open("test.c",O_RDONLY);
	if(fd1 < 0){
		perror("fd1open()");
		exit(0);
	}
	filechar = sum_file(fd1);

	fd2 = open("test.c",O_RDWR);
	if(fd2 < 0){
		perror("fd2open()");
		exit(0);
	}
	lseek(fd1,(off_t)offset_11,SEEK_SET);
	lseek(fd2,(off_t)offset_10,SEEK_SET);

	while(1){
		read_ret = read(fd1 ,buf, 1);
		if(read_ret <= 0){
			perror("read()");
			break;
		}
		
		write_ret = write(fd2 , buf , read_ret);
		if(write_ret <= 0){
			perror("read()");
			break;
		}
		
	}
	//把文件截断到少第10行的数据 
	ftruncate(fd2,(off_t)(filechar-(offset_11-offset_10)));
	close (fd1);
	close (fd2);

	exit(0);
}

int sumchar(int n){
	FILE* fd  =fopen("test.c","r");
	int count = 0 ;
	int line_ret = 0;
	char * line = NULL;
	size_t line_n;
	if(fd  == NULL ){
		perror("fopen()");
		exit(1);
	}
	printf("sumchar begin\n");
	n--;
	while(n >0 && getline(&line , &line_n ,fd) >0){
		count += strlen(line);
		n--;
	}
	fclose(fd);
	printf("sumchar over,char : %d\n",count);
	return count;
}


int sum_file(int fd){
	int count = 0;
	char buf[BUFSIZ];
	while(1){
		int read_ret = read(fd,buf , BUFSIZ);
		if(read_ret <= 0){
			break;
		}
		count += read_ret;
	}
	printf("sum_file over,charfile: %d\n",count);
	return count;
}
