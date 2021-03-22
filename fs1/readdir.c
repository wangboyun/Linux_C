#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>


int main(int argc , char *argv[]){

	if(argc != 2){
		fprintf(stderr,"usage不等于2个");
		exit(1);
	}
	
	DIR * d;
	struct dirent * dirp;
	char filename[256];
	
	d = opendir(argv[1]);
	if(d == NULL){
		perror("opendir()");
		exit(1);
	}
	while((dirp = readdir(d)) != NULL){
		if(strcmp(dirp->d_name , ".") == 0 || strcmp(dirp->d_name , "..") == 0)
			continue;
		strcpy(filename , dirp->d_name);
		printf("%s\n", filename);
	}

	exit(0);
}
