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
    struct sockaddr_in serv_addr, from_addr;
    socklen_t from_addr_sz;

    char buf[BUF_SIZE];
    int recv_cnt;
    FILE *fp;

    fp = fopen("udpfile_server.c", "rb");



    if(!fp) {
        perror("Fileopen error");
    }

    if (argc != 2) 
    {
        printf("Usage : %s <PORT> \n", argv[0]);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error !");
    }

    from_addr_sz = sizeof(from_addr);
    recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&from_addr, &from_addr_sz);
    fputs(buf, stdout);
    
    memset(buf, 0, BUF_SIZE); // 버퍼 초기화

    while ( (recv_cnt = fread(buf, 1, BUF_SIZE, fp)) > 0)
    {
        sendto(sock, buf, recv_cnt, 0 , (struct sockaddr *)&from_addr, sizeof(from_addr));
        memset(buf, 0, BUF_SIZE);
    }
    
        sendto(sock, "END", strlen("END"), 0 , (struct sockaddr *)&from_addr, sizeof(from_addr));
        

}

void error_handling(char *message) {
    perror(message);
    fputs(message, stderr);
}