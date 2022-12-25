#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <limits.h>
#include <string.h>

void My_atexit() {printf("\nExit Successfully\n", getpid());}
void Sig_Int(int num) 
{
    printf("\ngetpid =  %i getppid = %i num = %i strsignal(num) = (%s)\n", 
              getpid(),    getppid(),   num,     strsignal(num));
}
void Sig_Term(int num) 
{
    printf("\ngetpid =  %i getppid = %i num = %i strsignal(num) = (%s)\n", 
              getpid(),    getppid(),   num,     strsignal(num));
}
int main() 
{
    pid_t pid = fork();
    switch(pid)
    {
        case -1:
            printf("Smth_Error");
            return 1;
        break;
        case 0:
            printf(" CHILD: Мой PID -- %d\n"           , getpid());
            printf(" CHILD: PID моего родителя -- %d\n", getppid());
        break;
        default:
            printf("PARENT: Мой PID -- %d\n"       ,     getpid());
            printf("PARENT: PID моего потомка %d\n",     getppid());
        break;
    }  
    if (atexit(My_atexit) != 0)
        return 1;
    }   
    signal(SIGINT, Sig_Int);
    struct sigaction act; 
    act.sa_handler = Sig_Term;
    sigemptyset(&act.sa_mask);
    sigaction(SIGTERM, &act, NULL);
    while (1) {}
    return 0;
}
