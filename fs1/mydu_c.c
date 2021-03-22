#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <glob.h>
#include <dirent.h>

#define PATHSIZ 1024
#define FILENAMESIZ 256

static int64_t  mydu(const char* pathname);


int main(int argc , char * argv[]){

    //判断下传参数目
    if(argc < 2 ){
        fprintf(stderr , "参数少\n");
        exit(1);
    }

    //调用mydu（）函数
    printf("%ld      %s\n",mydu(argv[1])/2 , argv[1]);

    exit(0);
}


//利用一系列函数改写函数glob（）
static int64_t mydu(const char* pathname){
    struct stat statres;
	char path[PATHSIZ];
	int stat_re = lstat(pathname , &statres);
	DIR* d = NULL;
	struct dirent* dirp = NULL;
	int n = 0;
	char  filename[FILENAMESIZ];
	int i =0;
    if(stat_re < 0){
        perror("lstat()");
        exit(1);

    }

    //判断如果pathname不是目录文件就直接返回
    if(!S_ISDIR(statres.st_mode)){
       return statres.st_blocks;
    }
	
	int64_t sum = statres.st_blocks;

	
	if((d = opendir(pathname)) == NULL){
		perror("opendir()");
		exit(1);
	}
	// 将所有的文件名存在一个数组里
	
    while((dirp = readdir(d)) != NULL){
		if(strcmp(dirp->d_name , ".") == 0 || strcmp(dirp->d_name , "..") == 0)
            continue;
		strcpy(filename, pathname);
		strcat(filename, "/");
		strcat(filename , dirp->d_name);
		sum += mydu(filename);
    }
	
	
	closedir(d);
	return sum;

}
