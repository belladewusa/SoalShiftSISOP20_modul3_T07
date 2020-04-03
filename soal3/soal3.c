#include<sys/types.h>
#include<sys/wait.h>
#include <sys/stat.h> 
#include<stdio.h> 
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h> 

#define MAX 256 

  pthread_t tid[2000]; 

  char namafilelama[30]; 
  char namafilebaru[30]; 
  char ekstensi[10]; 
  char dirawal[20]; 
  char dirakhir[20];
  char f[] = "-f"; 
  char bintang = "*"; 
  char d[] = "-d";
  void* makedir(void *data);  
  void* move(void *data); 
  void* thread(void *arg);      


if (strcmp(argv[1],"-f")==0)
{
}
if else (strcmp(argv[1],"*")==0)
{
}
else (strcmp(argv[1], "-d") == 0)
{
}
