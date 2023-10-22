#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>



void error_handling(char *message);

int main(int argc, char *argv[]) {

    struct hostent *host;
    struct sockaddr_in addr;

    if(argc != 2) {
        printf("Usage : %s <IP>", argv[0]);
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);

    if(!host) {
        error_handling("gethost... error");
    }

    printf("Official name: %s", host->h_name);

    return 0;

    
}



void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}