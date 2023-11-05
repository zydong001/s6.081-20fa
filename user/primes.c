#include "kernel/types.h"
#include "user/user.h"

void recur(int lp[])
{
    close(lp[1]);
    int prime;
    int ret = read(lp[0], &prime, sizeof(int));
    if (ret < 0) {
        fprintf(2, "Fail to read prime.\n");
        return;
    } else if (ret == 0) {
        return;
    }

    printf("prime %d\n", prime);

    int np[2];
    pipe(np);

    if (fork() == 0) {
        close(lp[0]);
        close(np[1]);
        recur(np);
        exit(0);
    }

    close(np[0]);
    int num;
    while (read(lp[0], &num, sizeof(int)) == sizeof(int)) {
        if (num % prime != 0) {
            if (write(np[1], &num, sizeof(int)) != sizeof(int)) {
                fprintf(2, "Fail to write num(%d).\n", num);
                return;
            }
        }
    }
    close(lp[0]);
    close(np[1]);
    wait(0);
}

int main(int argc, char *argv[])
{
    const int N = 35;
    int lp[2];
    pipe(lp);

    if (fork() == 0) {
        recur(lp);
        exit(0);
    }

    close(lp[0]);
    for (int i = 2; i <= N; i++) {
        if (write(lp[1], &i, sizeof(int)) != sizeof(int)) {
            fprintf(2, "Fail to write num(%d) at the beginning.\n", i);
            exit(1);
        }
    }
    close(lp[1]);
    wait(0);
    exit(0);
}
