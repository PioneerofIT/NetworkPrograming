#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error_handling(char *message);

int main(int argc, char *argv[]) {
    int serv_sock; //Server 소켓
    int clnt_sock; //Client 소켓


    /* sockaddr_in 소켓 정보를 입락하는 라이브러리 구조체*/

    struct sockaddr_in serv_addr; //Server 주소 정보 구조체 변수
    struct sockaddr_in clnt_addr; //Client 주소 정보 구조체 변수
    socklen_t clnt_addr_size;

    char *message= "Hello World !";

    if(argc != 2) {
        printf("Usage : %s <port> \n", argv[0]);
        exit(1);
    }

    /*socket() -> bind() -> listen() -> accept()*/
    /*소켓 생성 파리미터 값은 프로토콜 관련 IPv4 , TCP, 0*/
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1) {
        error_handling("socket() error");
    }
    
    /*주소 초기화 과정*/
    memset(&serv_addr, 0, sizeof(serv_addr)); //memset 특정 메모리 초기화 메서드, serv_addr 구조체 모두 0으로 초기화
    serv_addr.sin_family = AF_INET; //IPv4 주소 패밀리 지정
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //host 바이트 순서에서 네트워크 바이트 순서로 변환
    serv_addr.sin_port = htons(atoi(argv[1])); //입력된 문자열 포트 값 정수값으로 변환 후 네트워크 바이트로 변환

    /*bind 함수는 소켓 파일디스크립터, 소켓 주소 구조체 포인터, 소켓 주소 구조체 크기 */
    /*bind 함수는 특정 타입의 소켓 주소 구조체인 (struct sockaddr *)를 요구 형변환 필요*/
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1 ) {
        error_handling("bind() error");
    }  

    /*한번에 5명까지 클라이언트 연결 요청 대기*/
    if(listen(serv_sock, 5)==-1) {
        error_handling("listen() error");
    }  

    /*accept() 함수는 인자 값으로 요청을 받아들이는 서버 소켓, 요청을 하는 클라이언트의 주소정보,클라이언트 주소정보의 크기*/
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
    if(clnt_sock ==-1){
        error_handling("accept() error");
    }

    /*client sock에 메세지를 전송*/
    write(clnt_sock, message, sizeof(message));

    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}


