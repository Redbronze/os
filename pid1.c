#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

time_t      rTime;
struct tm * timeInf;

int main(){
    pid_t pid;
    key_t key = ftok("mem",65);
    int mem = shmget(key,1024,0666|IPC_CREAT| S_IRUSR | S_IWUSR);
    time(&rTime);
    timeInf = localtime(&rTime);
    char* strT = asctime(timeInf);
    int   nsize = strlen(strT)+9;
    while (1){
        time(&rTime);
        timeInf = localtime(&rTime);
        char *str = (char*) shmat(mem, NULL, 0);
        strT = asctime(timeInf);
        snprintf(str, nsize, "[%d]: %s", getpid(), strT);
        printf(  "\nStr: %s\n",str);
        shmdt(str);
        sleep(1);
    }
    return 0;
}
