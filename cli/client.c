#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 4096

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s host port\n", argv[0]);
        return 1;
    }

    struct addrinfo hints = {0}, *result, *rp;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "%s\n", gai_strerror(s));
        return 1;
    }

    int sfd = -1;

    for (rp = result; rp; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(sfd);
    }

    freeaddrinfo(result);

    if (!rp) {
        fprintf(stderr, "Couldn't connect\n");
        return 1;
    }

    char buf[BUF_SIZE];

    while (fgets(buf, sizeof(buf), stdin)) {

        write(sfd, buf, strlen(buf));

        ssize_t n = read(sfd, buf, sizeof(buf)-1);

        if (n <= 0)
            break;

        buf[n] = '\0';

        printf("%s", buf);
    }

    close(sfd);
}

