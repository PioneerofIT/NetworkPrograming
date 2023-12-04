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
    
    FILE *fp;
    int recv_cnt;
    char buf[BUF_SIZE];

    fp = fopen("receive.dat", "wb");
    if(!fp) {
        perror("File Open error");
    }

    if (argc != 3) 
    {
        printf("Usage : %s <IP> <PORT> \n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    sendto(sock, "START", strlen("START"), 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    from_addr_sz = sizeof(from_addr);

    while (1)
    {
        memset(buf, 0, BUF_SIZE);
        recv_cnt = recvfrom(sock, buf, BUF_SIZE, 0 , (struct sockaddr *)&from_addr, &from_addr_sz);
        
        if(recv_cnt < 0) {
            perror("recvfrom() error");
            break;
        }
        
        if(!strcmp(buf,"END")) {
            break;
        }

        fwrite(buf, 1, recv_cnt, fp);
    }

    fclose(fp);
    close(sock);

    return 0;
    
    
}

void error_handling(char *message) {
    perror(message);
    fputs(message, stderr);
}