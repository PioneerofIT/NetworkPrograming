#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]) {

    int serv_sd, clnt_sd;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_adr_sz;

    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt; // 클라이언트부터 읽은 길이

    memset(buf, 0, BUF_SIZE);


    if(argc != 2) {
        printf("Usage : %s <PORT>", argv[0]);
        exit(1);
    }

    fp = fopen("file_server.c", "rb");
    serv_sd = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }

    if(listen(serv_sd, 5) ==1 ) {
        error_handling("listen() error");
    }


    clnt_adr_sz = sizeof(clnt_addr);
    if((clnt_sd=accept(serv_sd, (struct sockaddr *)&clnt_addr, &clnt_adr_sz)) == -1) {
        error_handling("accept() error");
    }

    while(1) {
        read_cnt = fread((void *)buf, 1, BUF_SIZE, fp); //1.읽어온 데이터 저장
        if(read_cnt < BUF_SIZE) {
            write(clnt_sd, buf, read_cnt);
            break;    
        }
        write(clnt_sd, buf, BUF_SIZE);
        
    }

    shutdown(clnt_sd, SHUT_WR);
    read(clnt_sd, buf, BUF_SIZE); 
    printf("Message from clinet : %s \n", buf);

    fclose(fp);
    close(clnt_sd);
    close(serv_sd);
    return 0;

}

void error_handling(char *message) {
    perror(message);
    exit(1);
}