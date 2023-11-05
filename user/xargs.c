#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/param.h"

int readLine(char *buf)
{
    char c;
    while (1) {
        if (read(0, &c, 1) == 0) {
            return 0;
        }
        if (c == '\n') {
            break;
        }
        *buf++ = c;
    }
    *buf = '\0';
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(2, "useage: xargs pattern [args...]\n");
        exit(1);
    }
    char path[20];
    strcpy(path, argv[1]);

    char *nargv[MAXARG];
    for (int i = 0; i < argc - 1; i++) {
        nargv[i] = argv[i + 1];
    }

    char buf[512];
    while (readLine(buf)) {
        if (fork() == 0) {
            nargv[argc - 1] = buf;
            exec(path, nargv);
            exit(0);
        } else {
            wait(0);
        }
    }
    exit(0);
}