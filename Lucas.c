#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/shm.h>

int lucasNumber(int n) { // get the number in the lucas series
    if (n == 0) return 2;
    if (n == 1) return 1;
    return lucasNumber(n - 1) + lucasNumber(n - 2);
}

int lucasNumberSum(int n) { // get the sum of all numbers from 0 to n of the lucas series and return it
    int total = 0;
    for (int i = 0; i < n; i++) {
        total += lucasNumber(i);
    }
    return total;
}

int main (int argc, char *argv[]) {
    if (argc != 2) fprintf(stderr, "Wrong number of arguments. Needs 1. Found %d\n", argc - 1);

    int num = atoi(argv[1]);
    printf("[Lucas][%ld]:The first 7 numbers of the Lucas series are :\n", (long)getpid());
    for (int i = 0; i < num; i++) {
        printf("%d ", lucasNumber(num));
    }
    printf("[Lucas] [%ld]: The nth number in the lucas series is %d\n", (long)getpid(), lucasNumber(num - 1));
    printf("[Lucas] [%ld]: The sum of the first %d numbers of the Lucas series is %d\n", (long)getpid(), num, lucasNumberSum(num));

    if (lucasNumberSum(num) > 50) { // if its sum is greater than 50 then just give the number passes in command line
        int fd = shm_open(argv[2], O_RDWR, 0666);
        char *pipePointer = (char *) mmap(NULL, 32, PROT_WRITE, MAP_SHARED, fd, 0);
        sprintf(pipePointer, "%d", num);
    } else {                         // otherwise, give it the sum
        int fd = shm_open(argv[2], O_RDWR, 0666);
        char *pipePointer = (char *)mmap(NULL, 32, PROT_WRITE, MAP_SHARED, fd, 0);
        sprintf(pipePointer, "%d", lucasNumberSum(num));
    }
}