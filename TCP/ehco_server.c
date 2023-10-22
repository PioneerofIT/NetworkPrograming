#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 1024
void error_handling(char *message);


int main(int argc, char *argv[]) {
    
    char message[BUF_SIZE]; //스트림을 통해 전달할 메세지
    int str_len, i; //메세지 길이

    int serv_sock, clnt_sock; //서버소켓과 클라이언트 소켓 변수
    struct sockaddr_in serv_addr; //Server 주소정보 구조체 변수
    struct sockaddr_in clnt_addr; //Client 주소정보 구조체 변수
    socklen_t clnt_adr_sz; // 클라이언트 주소정보 길이
    
    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }
    /*서버 소켓 생성*/
    serv_sock = socket(PF_INET,SOCK_STREAM, 0);
    if(serv_sock == -1) {
        error_handling("socket() error");
    }

    //주소 정보 초기화
    memset(&serv_addr, 0 , sizeof(serv_addr));
    serv_addr.sin_family = AF_INET; // IPv4 주소정보
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //로컬로 들어오는 모든 주소처리
    serv_addr.sin_port = htons(atoi(argv[1])); //파라미터로 받는 포트

    //소켓 주소정보 할당
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==-1) {
        error_handling("bind() error");
    }

    //클라이언트 허용 대기 접속 수
    if(listen(serv_sock, 5) == -1) {
        error_handling("listen() error");
    }

    //클라이언트 주소정보 길이
    clnt_adr_sz = sizeof(clnt_addr);
    for(i=0; i < 5; i++) {
        
        //일대일 연결
        //클라이언트의 연결 요청 시 clnt_addr정보 할당
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_adr_sz);
        if(clnt_sock == -1) {
            error_handling("clnt_sock error");
        }

        else {
            printf("Connect client : %d", i);
        }

        while((str_len = read(clnt_sock, message, BUF_SIZE)) !=0 ){
            write (clnt_sock, message, str_len);
        }

        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}


void error_handling(char *message) {
    fputs(message, stdout);
    fputc('\n', stderr);
    exit(1);
}