#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

void error_handling(char *message);

int main(int argc, char *argv[]) {

    int sock; //

    struct sockaddr_in serv_addr; //서버 주소
    char message[30];
    int str_len;


    if(argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    /*소켓의 기본적은 특성을 정의*/
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error");
    }

    /*서버의 주소와 포트번호 초기화  클라이언트는 서버의 포트와 주소를 알아야함*/
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    /*Server에 연결 요청*/
    /*클라이언트는 OS에서 IP와 PORT를 자동으로 할당 */
    if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1) {
        error_handling("connect() error");
    }

    /*Server 소켓에서 message를 읽어온 후 str_len에 메세지의 바이트 크기 저장*/
    str_len = read(sock, message, sizeof(message)-1);
    if(str_len == -1) {
        error_handling("read() error");
    }

    printf("Message from Server : %s \n", message);
    close(sock);
    return 0;
    
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

