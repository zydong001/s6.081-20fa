#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2];
    char buf[100];

    pipe(p);
    if (fork() == 0) {
        if (read(p[0], buf, sizeof(buf)) != 1) {
            fprintf(2, "Fail to read byte from parent.\n");
            exit(1);
        }
        close(p[0]);
        printf("%d: received ping\n", getpid());

        if (write(p[1], "a", 1) != 1) {
            fprintf(2, "Fail to write byte to parent.\n");
            exit(1);
        }
        close(p[1]);

        exit(0);
    }

    if (write(p[1], "a", 1) != 1) {
        fprintf(2, "Fail to write byte to child.\n");
        exit(1);
    }
    close(p[1]);

    wait(0);
    if (read(p[0], buf, sizeof(buf)) != 1) {
        fprintf(2, "Fail to read byte from child.\n");
        exit(1);
    }
    close(p[0]);
    printf("%d: received pong\n", getpid());

    exit(0);
}
