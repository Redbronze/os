#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

time_t rawtime;
struct tm * timeinfo;

void writer(int f) {
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char* str = asctime(timeinfo);
    int new_size = strlen(str)+15;
    char result[new_size];
    snprintf(result, new_size, "[%d]: %s", getpid(), str);
    write(f, result, strlen(result));
}
void reader(int f) {
    sleep(5);
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    char* str = asctime(timeinfo);
    char* buf = calloc(sizeof(char),32);
    read(f, buf, 32);
    printf("%s",buf);
    free(buf);
    printf("\n[%d]: ", getpid());
    printf("%s\n", str);
}
int main() {
    int f[2];
    pipe(f);
    pid_t CP = fork();
    switch (CP) {
    case -1:
        printf("fork error\n\a");
        exit(EXIT_FAILURE);
        break;
    case 0:
        close(f[1]); 
        reader(f[0]);
        close(f[0]);
        break;
    default:
        close(f[0]); 
        writer(f[1]);
        close(f[1]);
        break;
    }
    wait(NULL);
    printf("FIFO start\n");
    sleep(5);
    mkfifo("fifo.fifo", 0666);
    int id;
    CP = fork();  
    switch (CP) {
    
    case -1:
        printf("Fork error");
        exit(EXIT_FAILURE);
        break;
    case 0:
        id = open("fifo.fifo", O_RDONLY);
        reader(id);
        close(id);
        break;
    default:
        id = open("fifo.fifo", O_WRONLY);
        writer(id);
        close(id);
        break;
    }
    remove("fifo.fifo");
    wait(NULL);
    kill(CP,SIGTERM);
    
    return 0;
}
