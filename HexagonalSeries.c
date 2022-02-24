#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>

int hexNum(int n) {
    return (((2 * n) * (2 * n - 1)) / 2);
}
int hexSum (int n) { // recursively returns the sum of all hexagonal numbers from 1 to n
    if (n == 1) return 1;
    return hexNum(n) + hexSum(n -1);
}
int main (int argc, char **argv) {
    if (argc != 2) fprintf(stderr, "Wrong number of arguments. Needs 1. Found %d\n", argc - 1);
    int num = atoi(argv[1]);
    printf("[HexagonalSeries][%ld]:The first %d numbers of the Hexagonal series are :\n", (long)getpid(), num);
    for (int i = 0; i < num; i++) {
        printf("%d ", hexNum(num));
    }
    printf("[HexagonalSeries] [%ld]: The nth number in the Hexagonal series is %d\n", (long)getpid(), hexNum(num));
    printf("[HexagonalSeries] [%ld]: The sum of first %d hexagonal numbers is:%d\n", (long)getpid(), num, hexSum(num));

    if (hexSum(num) > 100) {// if it's greater than 100 just give it the number that was passed
        int fd = shm_open(argv[2], O_RDWR, 0666);
        char *pipePointer = (char *) mmap(NULL, 32, PROT_WRITE, MAP_SHARED, fd, 0);
        sprintf(pipePointer, "%d", num);
    } else {                  // otherwise, give it the sum
        int fd = shm_open(argv[2], O_RDWR, 0666);
        char *pipePointer = (char *) mmap(NULL, 32, PROT_WRITE, MAP_SHARED, fd, 0);
        sprintf(pipePointer, "%d", hexSum(num));
    }
}