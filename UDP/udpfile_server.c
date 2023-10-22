#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


#define BUF_SIZE 60
void error_handling(char *message);


int main(int argc, char *argv[]) {

    int sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_sz;

    char buf[BUF_SIZE];
    FILE *fp;
    int recv_cnt;

    fp = fopen("udpfile_server.c", "rb");
    if (argc != 2)
    {
        printf("Usage : %s <PORT>", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr))==-1) {
        error_handling("bind() error");
    }

    //클라이언트로 부터 시작 메세지를 받음
    recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&clnt_addr, &clnt_addr_sz); //주소 크기값을 주소값으로 받는 이유는 발송자의 주소크기는 변할 수 있기 떄문
    //위에서 받은 메세지를 초기화 해줘야함
    memset(buf, 0,BUF_SIZE);
    
    if(!fp) { //파일 열기 실패시 에러처리
        perror("File open error");
        close(sock);
        return 1;
    }

   while ((recv_cnt = fread(buf, 1, BUF_SIZE, fp)) > 0) { //읽은 파일의 바이트 수를 저장 0보다 작다면 전송 중단
        sendto(sock, buf, recv_cnt, 0, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr));// buf저장되어 있는 파일 스크립트 전송 
        memset(buf, 0, BUF_SIZE); //보낸 후 버퍼 초기화
    }
    
    sendto(sock, "END_OF_FILE", strlen("END_OF_FILE"), 0, (struct sockaddr *)&clnt_addr, sizeof(clnt_addr)); //파일 전송 종료를 알림
    
    memset(buf, 0,BUF_SIZE); //버퍼 초기화
    recvfrom(sock, buf, BUF_SIZE, 0, (struct sockaddr *)&clnt_addr, &clnt_addr_sz); //클라이언트의 메세지를 기다림

    if(strcmp(buf, "THANK_YOU_SERVER") == 0) //잘 받았다는 메세지가 도착하면 소켓종료
    {
        printf("클라이언트로 부터 감사메세지를 받았습니다. \n");
    }

    fclose(fp);
    close(sock);
    return 0;


}

void error_handling(char *message) {
    fputs(message, stderr);
    perror(message);
    exit(1);
}