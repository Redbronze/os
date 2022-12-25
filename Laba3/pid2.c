#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>

time_t      rTime;
struct tm * timeInf;

int main(){
    pid_t pid;
    key_t key = ftok("mem",65);
    int mem = shmget(key,1024,0666|IPC_CREAT);
    time(&rTime);
    timeInf = localtime(&rTime);
    char* strT = asctime(timeInf);
    while (1){
        time(&rTime);
        timeInf=localtime(&rTime);
        strT=asctime(timeInf);
        char*str=(char*)shmat(mem, NULL ,0);
        printf("\n%s",str);
        printf("\n[%d]: %s", getpid(), strT);
        shmdt(str);
        sleep(1);
    }
    shmctl(mem,IPC_RMID,NULL);
    return 0;
}
