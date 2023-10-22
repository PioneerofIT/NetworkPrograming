#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]) {
    int sd;
    struct sockaddr_in serv_addr;

    FILE *fp;
    char buf[BUF_SIZE];
    int read_cnt; //서버로 부터 읽은 길이

    if(argc != 3) {
        printf("Usage : %s <IP> <PORT>", argv[0]);
        exit(1);
    }
    fp = fopen("receive.dat", "wb");
    sd = socket(PF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while ((read_cnt = read(sd, buf, BUF_SIZE)) != 0) //파일 디스크립터, 저장변수, 읽을 바이트 수
    {
        fwrite((void *)buf, 1, read_cnt, fp); //buf에 저장된 데이터 쓰기, 1바이트 단위, read_cnt 읽은 크기만큼, fp파일에
    }
    
    puts("Received file data");
    write(sd, "Thank you", 10);
    fclose(fp);
    close(sd);

    return 0;

}



void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}