#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

void error_handling(char *message);
#define BUF_SIZE 1024


int main (int argc, char *argv[]) {

    int sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt;


    if(argc != 3) {
        printf("Usage : %s <IP> <PORT>", argv[0]);
        exit(0);
    }

    fp = fopen("receive.dat", "wb");
    if(!fp) {
        perror("File open error");
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1 ){
        error_handling("connet error");
    }

    else {
        fputs("Connect !\n", stdout);
    }

    while ((read_cnt = read(sock, buf, BUF_SIZE)) != 0 ) {
        fwrite(buf, 1, read_cnt, fp);

    }

    fputs("Recevie 완료 \n", stdout);
    write(sock, "THANK YOU !", strlen("THANK YOU !"));
    
    fclose(fp);
    close(sock);

    return 0;
    

}

void error_handling(char *message) {
    perror(message);
    fputs(message, stderr);
}