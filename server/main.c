#include "common.h"
#include "protocol.h"
#include "server.h"
#include "data-type.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define DS_SS_IMPLEMENTATION
#define DS_IO_IMPLEMENTATION
#include "ds.h"
#include <stdio.h>
#include <netdb.h>

#define MAX_LEN 1024

#define BUF_SIZE 500

int main(int argc, char *argv[])
{
    struct addrinfo          hints;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s port\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; // TCP
    hints.ai_flags = AI_PASSIVE;    /* For wildcard IP address */
    hints.ai_protocol = 0;          /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int s;
    struct addrinfo *result;

    s = getaddrinfo(NULL, argv[1], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    int sfd;
    struct addrinfo *rp;
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;                  /* Success */

        close(sfd);
    }
    freeaddrinfo(result);           /* No longer needed */

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not bind\n");
        exit(EXIT_FAILURE);
    }

    if (listen(sfd, 10) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }


    int cfd;
    struct sockaddr_storage client_addr;
    socklen_t client_addr_len;
    char buffer[BUF_SIZE];
    ssize_t res;
    for (;;) {
        client_addr_len = sizeof(client_addr);
        cfd = accept(sfd, (struct sockaddr *) &client_addr, &client_addr_len);
        if (cfd == -1) {
            perror("accept");
            continue;  // seguí esperando otras conexiones, no mates el server
        }

        // opcional: saber quién se conectó
        char host[NI_MAXHOST], service[NI_MAXSERV];
        getnameinfo((struct sockaddr *) &client_addr, client_addr_len,
                    host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
        printf("Conexión de %s:%s\n", host, service);

        res = read(cfd, buffer, BUF_SIZE);
        if (res == -1) {
            perror("read");
            close(cfd);
            continue;
        }

        unsigned int buffer_len = (unsigned int) res;
        printf("Client (%u): %.*s\n", buffer_len, buffer_len, buffer);

        ds_string_slice request, token;
        ds_string_slice_init(&request, buffer, buffer_len);
        ds_string_slice_tokenize(&request, ' ', &token);
        char *verb = NULL;
        ds_string_slice_to_owned(&token, &verb);
        if (strcmp(verb, "GET") != 0) {
            fprintf(stderr, "Unsupported method\n");
            close(cfd);
            free(verb);
            continue;
        }

        ds_string_slice_tokenize(&request, ' ', &token);
        char *path = NULL;
        ds_string_slice_to_owned(&token, &path);
        printf("Path requested: %s\n", path);
        // el protocol venia con un \r antes del \n
        ds_string_slice_tokenize(&request, '\n', &token);
        if (token.len > 0 &&
            token.str[token.len - 1] == '\r') {
            token.len--;
        }
        char *protocol = NULL;
        ds_string_slice_to_owned(&token, &protocol);
        printf("Protocol: %s\n", protocol);


        char *content = NULL;
        int content_len = ds_io_read_file(path+1, &content);

        char *resp_code = "200 OK";
        if(content_len == -1){
            perror("No file");
            resp_code = "404 Not Found";
            content = "404 Not Found\n";
            content_len = strlen(content);
        }
        int response_len = snprintf(
            NULL,
            0,
            "%s %s\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "\r\n"
            "%s",
            protocol,
            resp_code,
            content_len,
            content
        );
        char *response = calloc(response_len+1, sizeof(char));
        snprintf(
            response,
            response_len+1,
            "%s %s\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %d\r\n"
            "\r\n"
            "%s",
            protocol,
            resp_code,
            content_len,
            content
        );
        printf("%s\n", response);
        write(cfd, response, response_len);
        res = close(cfd);
        if (res==-1){
            perror("close");
            return -1;
        }
        free(response);

    }
        res = close(sfd);
        if (res==-1){
            perror("close");
            return -1;
        }


    return 0;
}
