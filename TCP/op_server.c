#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


#define BUF_SIZE 1024
#define OPSZ 4

void error_handling(char *message);
int calculate(int opnum, int opnds[], char op);


int main(int argc, char *argv[]) {

    /*서버와 관련된 변수 선언*/
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_addr_size; //accept() 에서 클라이언트 주소정보를 받아야하므로 선언.

    /*데이터 송수신에 관련된 변수 선언*/
    char opinfo[BUF_SIZE];
    int result, opnd_cnt, i;
    int recv_len, recv_cnt;

    if(argc != 2) {
        printf("Usage : %s <port>", argv[0]);
        exit(1);
    }
    
    /*server 소켓 생성*/
    serv_sock =socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1 ){
        error_handling("socket() error");
    }

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY); //Host TO Network Long
    serv_addr.sin_port = htons(atoi(argv[1])); // 포트는 바이트 값을 기대

    /*생성한 소켓과 주소 정보를 바인딩*/
    /*bind sockaddr* 의 유형을 기대하기 때문에 형변환을 해줘야함. */
    if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1) {
        error_handling("bind() error");
    }
    
    if(listen(serv_sock, 5) ==-1) {
        error_handling("listen() error");
    }
    

    clnt_addr_size = sizeof(clnt_addr);
    for(i=0; i<5; i++) {

        opnd_cnt=0; //피연산자 갯수
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        
        read(clnt_sock, &opnd_cnt, 1);
        recv_len=0;

        while ((opnd_cnt*OPSZ+1) > recv_len) //피연산자 갯수 * 정수형 4바이트 +1 연산자:char 문자
        {
            recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE-1);
            recv_len += recv_cnt;

        }

        result = calculate(opnd_cnt, (int *)opinfo, opinfo[recv_len-1]); //피연산자 갯수, 피연산자, 연산자
        write(clnt_sock, (char *)&result, sizeof(result));
        close(clnt_sock);

    }
    close(serv_sock);
    return 0;

}


int calculate(int opnum, int opnds[], char op) {
    int result = opnds[0];
    int i;

    switch (op)
    {
    case '+':
        for(i=1; i<opnum; i++) result += opnds[i];
        break;
    case '-':
         for(i=1; i<opnum; i++) result -= opnds[i];
         break;
    case '*':
         for(i=1; i<opnum; i++) result *= opnds[i];
         break;
    }
    return result;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}