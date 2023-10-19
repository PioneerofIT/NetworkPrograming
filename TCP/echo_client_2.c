#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[]) {
    
    int sock;
    char message[BUF_SIZE];
    int str_len, recv_len,recv_cnt;
    struct sockaddr_in serv_addr;

    if(argc != 3) {
        printf("Usage : %s <IP> <PORT> ", argv[0]);
        exit(1);
    }

    sock =socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1) {
        error_handling("socket() error");
    }

    //주소 체계 초기화
    memset(&serv_addr, 0, sizeof(serv_addr));
    
    //주소 체계 다시 설정
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    //Server와 커넥트
    if(connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) ==-1) {
        error_handling("connect() error !");
    } 
    else {
        printf("Connect............. \n");
    }

    /* 주소 설정 및 연결완료*/
    /* 데이터를 읽고 쓰는 부분*/
    
    /* TCP의 특성상 서버는 데이터 경계없이 전송 -> 클라이언트에서 받는 데이터의 경계 지정해서 수신*/
    /* write() 함수 파라미터 = 디스크립터, 쓸 데이터가 저장된 버퍼 포인터, 쓸 최대 바이트 수*/
    /* 성공적으로 쓴 바이트 수 반환 */

    /* read()함수 성공적으로 읽은 바이트 수 반환 */
    while (1)
    {
        fputs("Input q or Q to exit \n", stdout);
        fgets(message, BUF_SIZE, stdin); //message 누적 값 초기화

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")) {
            break;
        }
        str_len = write(sock, message, strlen(message)); //message 크기 바이트가 str_len에 저장

        recv_len = 0;
        while (recv_len < str_len) //str_len 쓴 바이트 크기 만큼 읽겠다는 뜻
        {
            recv_cnt = read(sock, &message[recv_len],BUF_SIZE-1); // message 의 읽은 바이트 저장 
                                                                // BUF_SIZE - 1 의 의미는 안전하게 널문자를 추가 하기 위함
                                                                // 오버플로우 방지
            if(recv_cnt == -1) {
                error_handling("read() error");
            }
            recv_len+= recv_cnt; //현재 까지 받은 데이터 길이 업데이트

        }
        message[recv_len] = 0; //마지막 문자열의 NULL추가 recv_len은 읽은 크기값
        printf("Message from Server : %s", message);
        

    }

    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
