#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"
#define NULL ((void *)(-1))
#define EOF (-1)

int
getchar() {
    char buf;
    if(read(0, &buf, sizeof(char))) {
        // printf("Debug: getchar buf %c\n", buf);
        return buf;
    } else {
        return EOF;
    }
}

char *
gets_s(char *s, int size) {
    int c = getchar();
    if(c == EOF) {
        return NULL;
    }

    int i = 0;
    do {
        if(c == '\n') {
            break;
        }
        s[i] = c;
        i++;
        if(i == size) {
            break;
        }
    }while((c = getchar()) != EOF);
    s[i] = 0;
    return s;
}

int
main(int argc, char *argv[]) {
    int status, i;
    
    if(argc < 2) {
        fprintf(2, "Usage: xargs [command]\n");
        exit(0);
    }

    char *buf[MAXARG+1];

    for(i = 1;i < argc;i++) {
        buf[i - 1] = argv[i];
    }

    // printf("Debug:entering main while loop\n");
    char buf2[1024];
    int args = argc - 1;
    while((gets_s(buf2, 1024)) != NULL) {
            // printf("Debug: buf2: %s\n", buf2);
            int j = 0;
            int l = 0;
            // parse space
            for(l = j; buf2[l] == ' ';l++) {
                ;
            }
            // printf("Debug: first l:%d\n", l);
            j = l;

            for(int k = j; ;k++) {
                if(buf2[k] == '\0' || buf2[k] == ' ') {
                    char *s1 = malloc((k-j+1)*sizeof(char));
                    memcpy(s1, buf2 + j, k - j);
                    s1[k - j + 1] = '\0';
                    // printf("Debug: in while loop s1:%s\n", s1);
                    buf[args++] = s1;
                    if(args == MAXARG - 1) {
                        fprintf(2, "xargs: Too many arguments...\n");
                        exit(1);
                    }
                    for(; buf2[k] != 0 || buf2[k] == ' ';k++) {
                        ;
                    }
                    j = k;
                }
                if(buf2[k] == 0) {
                    break;
                }
            }
    }
    buf[args] = 0;

    if(fork() == 0) {
        wait(&status);
        if(status == 0) {
            exit(0);
        } else {
            exit(1);
        }
    } else {
        // printf("Debug: exec command:");
        // for(int i = 0;buf[i] != 0;i++) {
        //     printf("%s ", buf[i]);
        // }
        // printf("\n");
        exec(buf[0], buf);
    }
    exit(0);
}