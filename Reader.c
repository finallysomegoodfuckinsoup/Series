#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

int main (int argc, char *argv[]) {
    if (argc != 3) fprintf(stderr, "[Reader]: Wrong number of arguments, usage requires 2, found %d", argc - 1);
    int pipe_ref = atoi(argv[2]);
    FILE *f = fopen(argv[1], "r");
    if (f == NULL) {
        fprintf(stderr, "File failed to open");
        return 1;
    }

    char buf[256];
    int run_sum = 0;
    int tmp = 0;
    printf("Hello from reader");
    while (fgets(buf, sizeof(buf), f)) {
        tmp = atoi(buf);
        run_sum += tmp;
    }
    sprintf(buf, "%d", run_sum);
    write (pipe_ref, buf, sizeof(buf));
    return 0;
}