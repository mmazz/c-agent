#include "common.h"
#include "protocol.h"
#include "server.h"
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#define DS_SS_IMPLEMENTATION
#define DS_IO_IMPLEMENTATION
#include "ds.h"

#define MAX_LEN 1024
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
        char buffer[MAX_LEN] = {0};

        int result = read(cfd, buffer, MAX_LEN);
        if (result == -1){
            perror("read");
            continue;
        }

        unsigned int buffer_len = result;
        printf("Client (%u): %s", buffer_len, buffer);


        ds_string_slice request, token;
        ds_string_slice_init(&request, buffer, buffer_len);
        ds_string_slice_tokenize(&request, ' ', &token);
        char *verb = NULL;
        ds_string_slice_to_owned(&token, &verb);
        if(strcmp(verb, "GET")!= 0){
            perror("not get");
            // TODO respond with 400
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
        int response_len = snprintf(NULL, 0, "%s 200 OK\nContent-Type: text/html\nContent-Length: %d\n\n%s", protocol,  content_len, content);
        char *response = calloc(response_len+1, sizeof(char));
        snprintf(response, response_len + 1, "%s 200 OK\nContent-Type: text/html\nContent-Length: %d\n\n%s", protocol, content_len, content);

        write(cfd, response, response_len);
    }
        result = close(sfd);
        if (result==-1){
            perror("close");
            return -1;
        }

    return 0;
}
