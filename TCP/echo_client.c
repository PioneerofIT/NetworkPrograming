
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]) {
    
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_addr;

    if(argc != 3) {
        printf("Usage : %s <IP> <PORT> ", argv[0]);
        exit(1);
    }

    sock =socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==-1) {
        error_handling("connect() error !");
    } 
    else {
        printf("Connect............. \n");
    }

    /* 주소 설정 및 연결완료*/
    /* 데이터를 읽고 쓰는 부분*/

    while (1)
    {
        fputs("Input q or Q to exit \n", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        write(sock, message, strlen(message));
        str_len = read(sock, message, BUF_SIZE);
        message[str_len] =0;
        printf("Message from Server : %s", message);

    }

    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
