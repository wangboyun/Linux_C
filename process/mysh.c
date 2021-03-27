#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <glob.h>
#include <sys/wait.h>



//先显示命令行
static void pr_opt(void){
	printf("mysh@wyz $ ");
}

struct cmd_opt {
	glob_t globres;
};

//解析命令行
static  void  parsing(char *line ,struct cmd_opt * res){
	char *tok;
	int i = 0;
	//切割字符串
	while(1){
		tok = strsep(&line , " \t\n");
		if(tok == NULL){
			break;
		}
		//会出现空格连输的情况  ，    ；
		if(tok[0] == '\0'){
			continue;
		}
		
		//将tok 存起来 , 第一次glob时无追加，之后每一次都追加
		glob(tok,GLOB_NOCHECK|GLOB_APPEND* i, NULL,&res->globres);
		i = 1 ;
	}
	
}

int main(void){
	
	pid_t pid;
	char *lineptr = NULL;
	size_t line_size = 0; /* getline 函数的参数必须要先进行初始化*/
	struct cmd_opt cmd;
	 //解析命令行
	
	while(1){
		
		pr_opt(); //先显示命令行
	
		fflush(NULL); //刷新流
	
		if(getline(&lineptr , &line_size , stdin) < 0){
			perror("getline()");
			exit(1);
		} //获取命令行信息
	
	
		parsing(lineptr , &cmd ); 
		
		pid = fork();
		if(pid < 0){
			perror("pid()");
			exit(1);
		}
		
		else if( pid == 0){ //child
			//命令分为内部和外部命令 
			if(0){/*目前不能解析内部命令*/}
			else{	
				puts(cmd.globres.gl_pathv[0]);
				execvp(cmd.globres.gl_pathv[0] ,cmd.globres.gl_pathv );
				perror("execvp()");
				exit(1);
			}
		}
		else 
			//globfree(&globres);
			wait(NULL); //等着收尸
			
	}
	
	return 0;
}