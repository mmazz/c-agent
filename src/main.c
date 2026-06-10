#include "common.h"
#include "protocol.h"
#include "server.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    int cfd;
    socklen_t client_addr_size;
    if (sfd==-1){
        perror("socket");
        return -1;
    }

    struct sockaddr_in addr, client_addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8000);
   // addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    socklen_t addrlen = sizeof(addr);
    int result = bind(sfd, (struct sockaddr *) &addr,  addrlen);
    if (result==-1){
        perror("bind");
        return -1;
    }
    result = listen(sfd, 10);
    if (result==-1){
        perror("listen");
        return -1;
    }
    while(1){
        client_addr_size = sizeof(client_addr);
        cfd = accept(sfd, (struct sockaddr *) &client_addr, &client_addr_size);
        if (cfd==-1){
            perror("accept");
            return -1;
        }
        while (1) {
            char buffer[1024] = {0};

            ssize_t n = read(cfd, buffer, sizeof(buffer) - 1);

            if (n <= 0)
                break;

            buffer[n] = '\0';

            printf("Client: %s", buffer);

            printf("Respuesta: ");

            fgets(buffer, sizeof(buffer), stdin);

            write(cfd, "Server: ", 8);
            write(cfd, buffer, strlen(buffer));
        }

        result = close(sfd);
        if (result==-1){
            perror("close");
            return -1;
        }

    }
    return 0;
}
