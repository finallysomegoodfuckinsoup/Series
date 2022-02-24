#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>

float harmNum(int n) {
    return 1.0/n;
}
float harmSum(int n) { // recursively returns the sum of all harmonic numbers from 1 to n
    if (n <= 1) return 1;
    return harmNum(n) + harmSum(n - 1);
}

int main (int argc, char **argv) {
    int num = atoi(argv[1]);
    printf("[HarmonicSeries][%ld]:The first 7 numbers of the Harmonic series are :\n", (long)getpid());
    for (int i = 0; i < num; i++) {
        printf("%f ", harmNum(num));
    }
    printf("[HarmonicSeries][%ld]:The nth number in the harmonic series is : %f", (long)getpid(), harmNum(num));
    printf("[HarmonicSeries] [%ld]: The sum of the first %d numbers of the harmonic series is %f\n", (long)getpid(), num, harmSum(num));
    int ret = (int)harmSum(num);
    int fd = shm_open(argv[2], O_RDWR, 0666);
    char *pipePointer = (char *) mmap(NULL, 32, PROT_WRITE, MAP_SHARED, fd, 0);
    sprintf(pipePointer, "%d", ret);
}