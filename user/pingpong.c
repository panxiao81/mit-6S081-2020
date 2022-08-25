#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int p[2];
    if(pipe(p) == -1) {
        fprintf(2, "Error when opening pipe");
        exit(1);
    }
    char buf[1];
    int pid = fork();
    if(pid == 0) {
        for(;;) {
            if(read(p[0], buf, sizeof(buf))) {
                fprintf(1, "%d: received ping\n", getpid());
            }

            write(p[1], (char*)'1', sizeof(buf));
            exit(0);
        }

    } else {
        write(p[1], (char*)'1', sizeof(buf));
        wait(0);
        for(;;) {
            if(read(p[0], buf, sizeof(buf))) {
                fprintf(1, "%d: received pong\n", getpid());
            }
            exit(0);
        }
    }
}