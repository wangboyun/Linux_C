#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int sumchar(int n);
int sum_file(int fd);

int main(void){
	int offset_11 = sumchar(11);
    int offset_10 = sumchar(10);
    int fd;
	int fork_fd; // 创建进程的返回值
	int pipefd[2] ; //无名管道的读写标志
	char buf[BUFSIZ];
	int read_ret;int count = 0;
	fd = open("test.c", O_RDONLY);
	if(fd < 0){
		perror("open()");
		exit(1);
	
	}
	int filechar = sum_file(fd);

	//创建管道
	if(pipe(pipefd) < 0){
		perror("pipe()");
		exit(1);
	}
	
	//创建新进程
	fork_fd = fork();
	if(fork_fd < 0){
		perror("fork()");
		exit(1);
	}
	else if(fork_fd == 0){
		
		//子进程 ， 读管道内数据 ,关闭写管道
		close(pipefd[1]) ; //关闭写管道

		fd = open("test.c", O_RDWR);
		lseek(fd,(off_t)offset_10,SEEK_SET);
		//将管道数据写入buf中
		while(read(pipefd[0],buf,1)>0){
	
		//将buf数据写入文件中
			write(fd , buf , 1);
		}

		close(pipefd[0]);
	}
	else{
		//父进程 ,写管道，关闭读管道
		fd = open("test.c", O_RDONLY);
        lseek(fd,(off_t)offset_11,SEEK_SET);
        //将文件数据读入buf
      	while(1){
			//读文件数据到buf中
			read_ret = read(fd,buf,1);
	
			if(read_ret <= 0){
				perror("read()");
				exit(1);
			}
			count += read_ret;
		}  
	
		printf("管道内数据为：%s\n",buf);
		close(pipefd[0]); //关闭读管道
		write(pipefd[1] , buf , count);
		
	}
	
	 //把文件截断到少第10行的数据 
     ftruncate(fd,(off_t)(filechar-(offset_11-offset_10)));

    close (fd);

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

