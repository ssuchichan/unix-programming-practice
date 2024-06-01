#include "head.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: <Server> <Port>\n");
        return -1;
    }

    server_epoll(argv);

    return 0;
}
