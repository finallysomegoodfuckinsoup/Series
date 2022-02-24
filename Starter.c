#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/shm.h>

//for testing
#include <inttypes.h>

int main(int argc, char *argv[]) {
    if (argc != 2) fprintf(stderr, "Wrong number of arguments. Needs 1. Found %d\n", argc - 1);
    pid_t pid;
    pid_t prPid = getpid();

    int n;
    int status;
    int p[2];
    if (pipe(p) < 0)
        fprintf(stderr, "Problem opening pipe!\n");

    char tmpbuf[10];
    char str[10];
    sprintf(tmpbuf, "%d", p[1]);
    pid = fork();
    int maxPrime;
    if (pid < 0) fprintf(stderr, "Error running child process\n");
    else if (pid == 0) { // child
        close(p[0]);
        execlp("./Reader", "reader", argv[1], tmpbuf, NULL);
    } else {
        wait(&status);
        close(p[1]);
        read(p[0], str, 10);
        printf("[Starter][%ld]: contents read from the read end pipe: %d\n", (long)prPid, atoi(str));
        n = atoi(str);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i/2; j++) {
            if (i % j == 0) continue;
            if (j == i/2-1) {
                maxPrime = i;
                break;
            }
        }
    }
    char primeString[256];
    sprintf(primeString, "%d", maxPrime);

    //arrays for running exec
    char *my_array[3] = {"lucas", "hexagonalseries", "harmonicseries"};
    char *my_array2[3] = {"./lucas", "./hexagonalseries", "./harmonicseries"};
    char *my_array3[3] = {"SHM_lucas", "SHM_hexagonalseries", "SHM_harmonicseries"};
    char *my_array4[3] = {"Lucas.c", "Hexagonalseries.c", "Harmonicseries.c"};

    int shms[3];
    void *voidarr[3];
    pid_t pidArray[3];



    for (int i = 0; i < 3; i++) {
        shms[i] = shm_open(my_array3[i], O_CREAT | O_RDWR, 0666);
        voidarr[i] = mmap(NULL, 32, PROT_WRITE, MAP_SHARED, shms[i], 0);
        pidArray[i] = fork();
        if (pidArray[i] < 0) {
            fprintf(stderr, "Error running child process %s", my_array[i]);
        } else if (pidArray[i] == 0) {
            execlp(my_array2[i], my_array4[i], my_array3[i], (char *) voidarr[i], NULL);
        } else { // parent
            //RECEIVING BUS ERROR ON LINE BELOW
            printf("[Starter]: %d and %s]\n", prPid, (char *)voidarr[i]);
            shm_unlink(my_array3[i]);
        }
    }
    for (int i = 0; i < 3; i++) {
        wait(&status);

        //TODO: CHANGE BELOW TO NOT USE WIFEXITED
        if (WIFEXITED(status)) {
            printf("[Starter][%ld] : Created Shared memory \"SHM_%s\" with FD: %d\n", (long)prPid, my_array[i], shms[i]);
        }

    }

    return 0;

}
//        printf("\nSet void[%d] to %s\n", i, (uint8_t*)voidarr[i]);

