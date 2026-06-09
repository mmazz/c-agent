#include "common.h"
#include "protocol.h"
#include "server.h"
#include <sys/socket.h>


int main(int argc, char* argv[]){
    int fd_s = socket(AF_INET, SOCK_STREAM, 0);
    if (fd_s==-1){
        perror("socket");
        return -1;
    }
    int result = bind(fd_s, , );
    printf("Hello world\n");
    return 0;
}
