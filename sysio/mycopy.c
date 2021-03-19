#include  <stdio.h>
#include  <errno.h>
#include  <stdlib.h>
 
int main(int argc,const char* argv[])
{
	  FILE* src_fd = NULL;
      FILE* det_fd = NULL;
       //打开传入参数的文件，传的参数是文件路径
       if(argc >3){
	  	  perror("传参数大于2\n");
		  exit(1);
      }
      //打开源文件以只读方式打开
      src_fd = fopen(argv[2],"r");
      if(src_fd == NULL){
          perror("src_fopen()");
          exit(1);
      }
 
      //打开目标文件以只写方式打开
      det_fd = fopen(argv[1],"w");
      if(det_fd == NULL){
		  fclose(src_fd);
          perror("det_fopen()");
          exit(1);
      }
      
      while(1){
          int data = fgetc(src_fd);
          if(data == EOF){
              perror("fget()");
             break;
          }
  
          fputc(data,det_fd);
  
      }
 	  fclose(src_fd);
	  fclose(det_fd); 
      return 0;
}
                               
