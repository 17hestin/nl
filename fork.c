#include<stdio.h>
#include <sys/types.h>
#include <unistd.h>
void main()
{
int pid;
pid = fork();
if(pid > 0)
{
printf("I am parent\n");
}
else
{
printf("I am child\n");
}
}
