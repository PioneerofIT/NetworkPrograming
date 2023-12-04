#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char *message);
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    int sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    int recv_cnt;
    char message[BUF_SIZE];

    if(argc != 3) {
        printf("Usage : %s <IP> <PORT>", argv[0]);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));


    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("Connet error");
    }

    else{
        printf("connet.......... \n");
    }

    while (1)
    {
        fputs("서버에 전송하고 싶은 메세지를 입력 :", stdout);
        fgets(message,BUF_SIZE, stdin);
        if(!strcmp(message, "END\n")) {
            write(sock, "END", strlen("END"));
            break;
        }
        write(sock, message, BUF_SIZE);
        memset(message, 0, BUF_SIZE);
        
        read(sock, message, BUF_SIZE);
        fputs(message, stdout);
        
    }

    close(sock);
    
    return 0;


}

void error_handling(char *message) {
    perror(message);
    fputs(message, stderr);
}