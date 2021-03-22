#include "myls.h"
/*
目前只支持路径为一个值的情况，非选项传参职位一个

*/

char* change_mode(long int st_mode,char c_mode[15] ){
	//判断文件类型
	memset(c_mode,0,15);
	if(S_ISREG(st_mode)){
		c_mode[0] = '-';
	}
	else if(S_ISDIR(st_mode)){
		c_mode[0] = 'd';
	}
	else if(S_ISCHR(st_mode)){
		c_mode[0] = 'c';
	}
	else if(S_ISBLK(st_mode)){
		c_mode[0] = 'b';
	}
	else if(S_ISFIFO(st_mode)){
		c_mode[0] = 'p';
	}
	else if(S_ISLNK(st_mode)){
		c_mode[0] = 'l';
	}
	else if(S_ISSOCK(st_mode)){
		c_mode[0] = 's';
	}
	
	//用户权限
	if(st_mode & S_IRUSR){
		c_mode[1] = 'r';
	}
	else 
		c_mode[1] = '-';
	if(st_mode & S_IWUSR){
		c_mode[2] = 'w';
	}
	else 
		c_mode[2] = '-';
	if(st_mode & S_IXUSR){
		c_mode[3] = 'x';
	}
	else 
		c_mode[3] = '-';
	
	//组权限转换
	if(st_mode & S_IRGRP){
		c_mode[4] = 'r';
	}
	else 
		c_mode[4] = '-';
	if(st_mode & S_IWGRP){
		c_mode[5] = 'w';
	}
	else 
		c_mode[5] = '-';
	if(st_mode & S_IXGRP){
		c_mode[6] = 'x';
	}
	else 
		c_mode[6] = '-';
	
	//其他组转换
	if(st_mode & S_IROTH){
		c_mode[7] = 'r';
	}
	else 
		c_mode[7] = '-';
	if(st_mode & S_IWOTH){
		c_mode[8] = 'w';
	}
	else 
		c_mode[8] = '-';
	if(st_mode & S_IXOTH){
		c_mode[9] = 'x';
	}
	else 
		c_mode[9] = '-';
	
	return c_mode;
}


void myls_t(char * p){ //显示非隐藏文件，当前目录解析文件
	//用readdir函数实现
	DIR *dirp;
	struct dirent * direntptr;
	dirp  = opendir(p);
	if(dirp == NULL){
		perror("opendir()");
		exit(1);
	}
	while((direntptr = readdir(dirp)) != NULL){
		//去除掉. .. 目录
		if(strcmp(direntptr->d_name,".") == 0 ||strcmp(direntptr->d_name,"..") == 0)
			continue;
		printf("%s\n" , direntptr->d_name);
	}
	closedir(dirp );
}

void myls_a(char * p){ //显示隐藏文件，当前目录解析文件
	//用readdir函数实现
	DIR *dirp;
	struct dirent * direntptr;
	dirp  = opendir(p);
	if(dirp == NULL){
		perror("opendir()");
		exit(1);
	}
	while((direntptr = readdir(dirp)) != NULL){
		
		//if(strcmp(direntptr->d_name,".") == 0 ||strcmp(direntptr->d_name,"..") == 0)
			//continue;
		printf("%s\n" , direntptr->d_name);
	}
	closedir(dirp );
}

void myls_n(char * p){
	DIR *dirp;
	struct dirent * direntptr;
	struct stat statres;
	char c_mode[15];
	char *mode_string ;
	char *time_string ;
	char *timeret;
	int linksize,filesize;
	int uid,gid;
	struct tm* tmptr;
	
	dirp  = opendir(p);
	if(dirp == NULL){
		perror("opendir()");
		exit(1);
	}
	while((direntptr = readdir(dirp)) != NULL){
		//去除掉. .. 目录
		if(strcmp(direntptr->d_name,".") == 0 ||strcmp(direntptr->d_name,"..") == 0)
			continue;
		if(lstat(direntptr->d_name,&statres) < 0){
			perror("lstat()");
			exit(1);
		}
		mode_string = change_mode(statres.st_mode ,c_mode);
		linksize = statres.st_nlink;
		uid = statres.st_uid;
		gid = statres.st_gid;
		filesize = statres.st_size;
		tmptr = localtime(&statres.st_ctime);
		strftime(time_string,256,"%-m 月 %d %R",tmptr);
		printf("%s %d %d %d %5d %-4s %s\n",
				mode_string,linksize,uid,gid,filesize,time_string,direntptr->d_name);
	}
	closedir(dirp );
}

void myls_l(char * p){
	DIR *dirp;
	struct dirent * direntptr;
	struct stat statres;
	struct passwd* passwduidptr ;
	struct group* groupidptr ;
	struct tm* tmptr;
	char c_mode[15];
	char *mode_string ;
	char *time_string ;
	char *timeret;
	char *uname ;
	char *gname ;
	int linksize,filesize;
	//int uid,gid;
	dirp  = opendir(p);
	if(dirp == NULL){
		perror("opendir()");
		exit(1);
	}
	while((direntptr = readdir(dirp)) != NULL){
		//去除掉. .. 目录
		if(strcmp(direntptr->d_name,".") == 0 ||strcmp(direntptr->d_name,"..") == 0)
			continue;
		if(lstat(direntptr->d_name,&statres) < 0){
			perror("lstat()");
			exit(1);
		}
		mode_string = change_mode(statres.st_mode ,c_mode);
		linksize = statres.st_nlink;
		passwduidptr = getpwuid( statres.st_uid);
		groupidptr = getgrgid(statres.st_gid);
		filesize = statres.st_size;
		tmptr = localtime(&statres.st_ctime);
		strftime(time_string,256,"%-m 月 %d %R",tmptr);
		printf("%s %d %s %s %5d %-4s %s\n",
				mode_string,linksize,passwduidptr->pw_name,groupidptr->gr_name,filesize,time_string,direntptr->d_name);
	}
	closedir(dirp );
}

int main(int argc , char* argv[]){
	int ch_opt;
	
	if(argc < 2 ){
		fprintf(stderr,"传参数少\n");
		exit(1);
	}
	
	while(1){
		ch_opt = getopt(argc , argv , "-lant");
		if(ch_opt < 0)
			break;
		switch(ch_opt){
			
				
			case('t'):
				//显示非隐藏文件，当前目录解析文件
				if(argc == 2){
					myls_t(".");
				}
				else if(argc >= 3){
					myls_t(argv[argc-1]);
				}
			break;
			case('a'): //显示隐藏文件，当前目录解析文件
				if(argc == 2){
					myls_a(".");
				}
				else if(argc >=3){
					myls_a(argv[argc-1]);
				}
				
			break;
			case('n'):
				if(argc == 2){
					myls_n(".");
				}
				else if(argc >= 3){
					myls_n(argv[argc-1]);
				}
			break;
			case 'l':
				if(argc == 2){
					myls_l(".");
				}
				else if(argc >= 3){
					myls_l(argv[argc-1]);
				}
			break;
			case 1:
				puts(argv[optind-1]);
			break;
			default:
			break;
		}
		
	}
	return 0;
}
