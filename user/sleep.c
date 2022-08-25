#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) 
{

    if (argc == 2)
    {
        sleep(atoi(argv[1]));
        exit(0);
    } else {
        fprintf(2, "Usage: sleep [ticks]\n");
        exit(1);
    }
}