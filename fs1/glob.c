#include <stdio.h>
#include <glob.h>
#include <unistd.h>
#include <stdlib.h>

#define PATNAME  "/etc/*"

int main(){

	//解析目录
	glob_t  pglob;

	int res = -1;
	int i = 0 ;
	res = glob(PATNAME , 0 , NULL , &pglob);
	
	if(res){
		perror("glob()");
		exit(1);
	}
	
	printf("目录文件内有文件数：%ld\n", pglob.gl_pathc);
	
	for(i = 1 ;i < pglob.gl_pathc ; i++){
		puts(pglob.gl_pathv[i]);
	
	}
	globfree(&pglob);

	exit(0);
}
