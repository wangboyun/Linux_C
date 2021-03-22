#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glob.h>

#define PATHSIZ 1024

static int64_t mydu(const char* pathname);

static int is_noloop(const char* pathname);

int main(int argc , char * argv[]){

	//判断下传参数目
	if(argc < 2 ){
		fprintf(stderr , "参数少\n");
		exit(1);
	}

	//调用mydu（）函数
	printf("%ld			%s\n",mydu(argv[1])/2 , argv[1]);

	exit(0);
}



static int64_t mydu(const char* pathname){
	struct stat statres;
	char path[PATHSIZ];
	glob_t globres;
	int i ;
	int64_t sum = 0;
	int stat_re = lstat(pathname , &statres);
	if(stat_re < 0){
		perror("lstat()");
		exit(1);
	
	}

	//判断如果pathname不是目录文件就直接返回
	if(!S_ISDIR(statres.st_mode)){
		return statres.st_blocks;
	}

	//这里是pathname为一个目录文件
	
	strncpy(path , pathname , PATHSIZ);
	strncat(path ,"/*",PATHSIZ);
	if(glob(path , 0, NULL ,&globres ) != 0){
		perror("glob()");
		exit(1);
	}
	
	strncpy(path , pathname , PATHSIZ);
    strncat(path ,"/.*",PATHSIZ);
   	if( glob(path , GLOB_APPEND, NULL ,&globres ) != 0){
		perror("glob()");
		exit(1);
	}
		
	sum = statres.st_blocks;
	//利用一个for循环进行递归解析
	for (i =0 ; i < globres.gl_pathc ; i++){
		if(is_noloop(globres.gl_pathv[i]))
			sum += mydu(globres.gl_pathv[i]);
	}

	globfree(&globres);
	return sum;
}

static int is_noloop(const char* pathname){

	char * ptr = strrchr(pathname , '/');
	if(strcmp(ptr , "/.") == 0 || strcmp(ptr, "/..")==0  )
		return 0;
	return 1;
}
