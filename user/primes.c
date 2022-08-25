#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define MAX_PRIME 35

void
prime(int *p) {
    close(p[1]);
    int primes;
    if(read(p[0], &primes, sizeof(primes)) != sizeof(primes)) {
        fprintf(2, "Error\n");
        exit(1);
    }
    printf("prime %d\n", primes);
    int left;
    //
    if(read(p[0], &left, sizeof(left))) {
        int p1[2];
        pipe(p1);
        if(fork() == 0) {
            prime(p1);
        } else {
            close(p1[0]);
            do {
                if(left % primes != 0) {
                    write(p1[1], &left, sizeof(left));
                }
            }while(read(p[0], &left, sizeof(left)));
            close(p[0]);
            close(p1[1]);
            wait(0);
        }
    }
    exit(0);

}

int
main() {
    int p[2];
    int status = 0;
    pipe(p);
    if(fork() == 0) {
        prime(p);
    } else {
        close(p[0]);
        for(int i = 2;i <= MAX_PRIME;i++) {
            if(write(p[1], &i, sizeof(i)) != sizeof(i)) {
                fprintf(2, "Error\n");
                exit(1);
            }
        }
        close(p[1]);
        wait(&status);
    }
    if(status) {
        exit(0);
    }
    exit(1);
}