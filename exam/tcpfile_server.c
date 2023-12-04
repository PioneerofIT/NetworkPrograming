#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char *message);
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
    
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    FILE *fp;
    char buf[BUF_SIZE];
    int recv_cnt;

    fp = fopen("tcpfile_server.c", "rb");
    if(!fp) {
        perror("file open error() \n");
    }

    if(argc != 2) {
        printf("Usage : %s <IP>", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==-1 ) {
        error_handling("bimd() error!");
    }

    if(listen(serv_sock, 5) ==-1) {
        error_handling("error listen ()");
    }

    clnt_addr_sz = sizeof(clnt_addr);

    if((clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz)) == -1 ){
        error_handling("accept() error");
    }

    while (1)
    {
        recv_cnt = fread(buf, 1, BUF_SIZE, fp);
        if(recv_cnt < BUF_SIZE){
            write(clnt_sock, buf, recv_cnt);
            break;
        }
        write(clnt_sock, buf, BUF_SIZE);
    }

    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, buf, BUF_SIZE);
    printf("Client message : %s ", buf);

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    
    return 0;
    
}


void error_handling(char *message) {
    perror(message);
    fputs(message, stderr);
}