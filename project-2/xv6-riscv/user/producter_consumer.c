#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

int 
producer()
{      
    int empty = sem_get(1);
    int full = sem_get(2);
    int mutex = sem_get(3);
    
    for(int i = 0; i < 5; i++){
        sem_wait(empty);
        sem_wait(mutex);
        int fd; 
        char data[10];
        fd = open("buffer.txt", O_RDONLY);
        read(fd, data, 10);
        close(fd);
        fd = open("buffer.txt", O_WRONLY);
        int value = atoi(data)+1;
        fprintf(fd, "%d", value);
        close(fd);
        printf("Producer: %d -> %d\n", value-1, value);
        sem_signal(mutex);
        sem_signal(full);
    }

    return 0;
}

int 
consumer()
{
    int empty = sem_get(1);
    int full = sem_get(2);
    int mutex = sem_get(3);

    for(int i = 0; i < 5; i++){
        sem_wait(full);
        sem_wait(mutex);
        int fd = open("buffer.txt", O_RDONLY);
        char data[10];
        read(fd, data, 10);
        close(fd);
        fd = open("buffer.txt", O_WRONLY);
        int value = atoi(data)-1;
        fprintf(fd, "%d", value);
        close(fd);
        printf("Consumer: %d -> %d\n", atoi(data), value);
        sem_signal(mutex);
        sem_signal(empty);
    }
    
    return 0;
}

int 
main(void)
{
    int fd = open("buffer.txt", O_CREATE | O_WRONLY);
    int data = 0;
    fprintf(fd, "%d", data);
    close(fd);

    sem_create(1, 5);
    sem_create(2, 0);
    sem_create(3, 1);

    for (int i = 0; i < 1; i++){
        if(fork() == 0) {
            int empty = sem_get(1);
            int full = sem_get(2);
            int mutex = sem_get(3);
            producer();
            sem_close(empty);
            sem_close(full);
            sem_close(mutex);
            exit(0);
        }else if (fork() == 0) {
            int empty = sem_get(1);
            int full = sem_get(2);
            int mutex = sem_get(3);
            consumer();
            sem_close(empty);
            sem_close(full);
            sem_close(mutex);
            exit(0);
        }else{
            wait(0);
            wait(0);
        }
    }
    
    return 0;
}
